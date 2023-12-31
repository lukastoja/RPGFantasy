// Copyright Luka Otović


#include "HUD/WidgetController/SpellMenuWidgetController.h"
#include "AbilitySystem/FantasyAbilitySystemComponent.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "Characters/FantasyPlayerState.h"

void USpellMenuWidgetController::BroadcastInitialValues()
{
	BroadcastAbilityInfo();
	SpellPointsChanged.Broadcast(GetFantasyPS()->GetSpellPoints());
}

void USpellMenuWidgetController::BindCallbacksToDependencies()
{
	GetFantasyASC()->AbilityStatusChanged.AddLambda([this](const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, int32 NewLevel)
	{
		if (SelectedAbility.Ability.MatchesTagExact(AbilityTag))
		{
			SelectedAbility.Status = StatusTag;
			bool bEnableSpellPoints = false;
			bool bEnableEquip = false;

			ShouldEnableButtons(StatusTag, CurrentSpellPoints, bEnableSpellPoints, bEnableEquip);

			FString Description;
			FString NextLevelDescription;
			GetFantasyASC()->GetDescriptionsByAbilityTag(AbilityTag, Description, NextLevelDescription);

			SpellGlobeSelectedDelegate.Broadcast(bEnableSpellPoints, bEnableEquip, Description, NextLevelDescription);
		}

		if (AbilityInfo)
		{
			FFantasyAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);
			Info.StatusTag = StatusTag;
			AbilityInfoDelegate.Broadcast(Info);
		}
	});

	GetFantasyASC()->AbilityEquipped.AddUObject(this, &USpellMenuWidgetController::OnAbilityEquipped);

	GetFantasyPS()->OnSpellPointsChangedDelegate.AddLambda([this](int32 SpellPoints)
	{
		SpellPointsChanged.Broadcast(SpellPoints);
		CurrentSpellPoints = SpellPoints;
		bool bEnableSpellPoints = false;
		bool bEnableEquip = false;

		ShouldEnableButtons(SelectedAbility.Status, CurrentSpellPoints, bEnableSpellPoints, bEnableEquip);

		FString Description;
		FString NextLevelDescription;
		GetFantasyASC()->GetDescriptionsByAbilityTag(SelectedAbility.Ability, Description, NextLevelDescription);

		SpellGlobeSelectedDelegate.Broadcast(bEnableSpellPoints, bEnableEquip, Description, NextLevelDescription);
	});
}

void USpellMenuWidgetController::SpellGlobeSelected(const FGameplayTag& AbilityTag)
{
	if (bWaitingForEquipedSelection)
	{
		const FGameplayTag SelectedAbilityType = AbilityInfo->FindAbilityInfoForTag(AbilityTag).AbilityType;
		StopWaitForEquipDelegate.Broadcast(SelectedAbilityType);
		bWaitingForEquipedSelection = false;
	}

	const FFantasyGameplayTags GameplayTags = FFantasyGameplayTags::Get();
	FGameplayAbilitySpec* AbilitySpec = GetFantasyASC()->GetSpecFromAbilityTag(AbilityTag);
	const int32 Spellpoints = GetFantasyPS()->GetSpellPoints();
	FGameplayTag AbilityStatus;

	bool bTagValid = AbilityTag.IsValid();
	bool bTagNone = AbilityTag.MatchesTag(GameplayTags.Abilities_None);
	bool bSpecValid = AbilitySpec != nullptr;

	if (!bTagValid || bTagNone || !bSpecValid)
		AbilityStatus = GameplayTags.Abilities_Status_Locked;
	else
		AbilityStatus = GetFantasyASC()->GetStatusFromSpec(*AbilitySpec);

	SelectedAbility.Ability = AbilityTag;
	SelectedAbility.Status = AbilityStatus;

	bool bEnableSpellPoints = false;
	bool bEnableEquip = false;

	ShouldEnableButtons(AbilityStatus, Spellpoints, bEnableSpellPoints, bEnableEquip);
	
	FString Description;
	FString NextLevelDescription;
	GetFantasyASC()->GetDescriptionsByAbilityTag(AbilityTag, Description, NextLevelDescription);

	SpellGlobeSelectedDelegate.Broadcast(bEnableSpellPoints, bEnableEquip, Description, NextLevelDescription);
}

void USpellMenuWidgetController::SpendPointButtonPressed()
{
	if(GetFantasyASC())
		GetFantasyASC()->ServerSpendSpellPoints(SelectedAbility.Ability);
}

void USpellMenuWidgetController::GlobeDeselect()
{
	if (bWaitingForEquipedSelection)
	{
		const FGameplayTag SelectedAbilityType = AbilityInfo->FindAbilityInfoForTag(SelectedAbility.Ability).AbilityType;
		StopWaitForEquipDelegate.Broadcast(SelectedAbilityType);
		bWaitingForEquipedSelection = false;
	}

	SelectedAbility.Ability = FFantasyGameplayTags::Get().Abilities_None;
	SelectedAbility.Status = FFantasyGameplayTags::Get().Abilities_Status_Locked;

	SpellGlobeSelectedDelegate.Broadcast(false, false, FString(), FString());
}

void USpellMenuWidgetController::EquipeButtonPressed()
{
	const FGameplayTag AbilityType = AbilityInfo->FindAbilityInfoForTag(SelectedAbility.Ability).AbilityType;

	WaitForEquipDelegate.Broadcast(AbilityType);
	bWaitingForEquipedSelection = true;

	const FGameplayTag SelectedStatus = GetFantasyASC()->GetStatusFromAbilityTag(SelectedAbility.Ability);
	if (SelectedStatus.MatchesTagExact(FFantasyGameplayTags::Get().Abilities_Status_Equipped))
		SelectedSlot = GetFantasyASC()->GetInputTagFromAbilityTag(SelectedAbility.Ability);
}

void USpellMenuWidgetController::SpellRowGlobePressed(const FGameplayTag& SlotTag, const FGameplayTag& AbilityType)
{
	if (!bWaitingForEquipedSelection) return;
	// check selected ability against the slot's ability type. (don't have an offensive spell in a passive slot and vice versa)
	const FGameplayTag& SelectedAbilityType = AbilityInfo->FindAbilityInfoForTag(SelectedAbility.Ability).AbilityType;
	if (!SelectedAbilityType.MatchesTagExact(AbilityType)) return;

	GetFantasyASC()->ServerEquipAbility(SelectedAbility.Ability, SlotTag);
}

void USpellMenuWidgetController::OnAbilityEquipped(const FGameplayTag& AbilityTag, const FGameplayTag& Status, const FGameplayTag& Slot, const FGameplayTag& PrevSlot)
{
	bWaitingForEquipedSelection = false;

	const FFantasyGameplayTags& GameplayTags = FFantasyGameplayTags::Get();

	FFantasyAbilityInfo LastSlotInfo;
	LastSlotInfo.StatusTag = GameplayTags.Abilities_Status_Unlocked;
	LastSlotInfo.InputTag = PrevSlot;
	LastSlotInfo.AbilityTag = GameplayTags.Abilities_None;
	AbilityInfoDelegate.Broadcast(LastSlotInfo);

	FFantasyAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);
	Info.StatusTag = Status;
	Info.InputTag = Slot;
	AbilityInfoDelegate.Broadcast(Info);

	StopWaitForEquipDelegate.Broadcast(AbilityInfo->FindAbilityInfoForTag(AbilityTag).AbilityType);
	SpellGlobeReassignedDelegate.Broadcast(AbilityTag);
	GlobeDeselect();
}

void USpellMenuWidgetController::ShouldEnableButtons(const FGameplayTag& AbilityStatus, int32 SpellPoints, bool& bShouldEnableSpellPoints, bool& bShouldEnableEquip)
{
	const FFantasyGameplayTags GameplayTags = FFantasyGameplayTags::Get();
	bShouldEnableSpellPoints = false;
	bShouldEnableEquip = false;

	if (AbilityStatus.MatchesTagExact(GameplayTags.Abilities_Status_Equipped))
	{
		bShouldEnableEquip = true;
		if (SpellPoints > 0) bShouldEnableSpellPoints = true;
	}
	else if (AbilityStatus.MatchesTagExact(GameplayTags.Abilities_Status_Eligible) && SpellPoints > 0)
		bShouldEnableSpellPoints = true;
	else if (AbilityStatus.MatchesTagExact(GameplayTags.Abilities_Status_Unlocked))
	{
		bShouldEnableEquip = true;
		if (SpellPoints > 0) bShouldEnableSpellPoints = true;
	}

}
