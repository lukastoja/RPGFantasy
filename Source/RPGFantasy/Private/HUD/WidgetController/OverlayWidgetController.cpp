// Copyright Luka Otović


#include "HUD/WidgetController/OverlayWidgetController.h"
#include "AbilitySystem/FantasyAttributeSet.h"
#include "AbilitySystem/FantasyAbilitySystemComponent.h"
#include "Characters/FantasyPlayerState.h"
#include "AbilitySystem/Data/LevelUpInfo.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	const UFantasyAttributeSet* FantasyAttributeSet = CastChecked<UFantasyAttributeSet>(AttributeSet);

	OnHealthChanged.Broadcast(FantasyAttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(FantasyAttributeSet->GetMaxHealth());
	OnManaChanged.Broadcast(FantasyAttributeSet->GetMana());
	OnMaxManaChanged.Broadcast(FantasyAttributeSet->GetMaxMana());
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	AFantasyPlayerState* FantasyPlayerState = CastChecked<AFantasyPlayerState>(PlayerState);

	FantasyPlayerState->OnXPChangedDelegate.AddUObject(this, &UOverlayWidgetController::OnXPChanged);
	FantasyPlayerState->OnLevelChangedDelegate.AddLambda(
		[this](int32 NewLevel)
	{
		OnPlayerLevelChangedDelegate.Broadcast(NewLevel);
	});

	const UFantasyAttributeSet* FantasyAttributeSet = CastChecked<UFantasyAttributeSet>(AttributeSet);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(FantasyAttributeSet->GetHealthAttribute()).AddLambda([this](const FOnAttributeChangeData& Data){ OnHealthChanged.Broadcast(Data.NewValue); });
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(FantasyAttributeSet->GetMaxHealthAttribute()).AddLambda([this](const FOnAttributeChangeData& Data) { OnMaxHealthChanged.Broadcast(Data.NewValue); });
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(FantasyAttributeSet->GetManaAttribute()).AddLambda([this](const FOnAttributeChangeData& Data) { OnManaChanged.Broadcast(Data.NewValue); });
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(FantasyAttributeSet->GetMaxManaAttribute()).AddLambda([this](const FOnAttributeChangeData& Data) { OnMaxManaChanged.Broadcast(Data.NewValue); });
	
	if (UFantasyAbilitySystemComponent* FantasyASC = Cast<UFantasyAbilitySystemComponent>(AbilitySystemComponent))
	{
		if (FantasyASC->bStartupAbilitiesGiven)
			OnInitializeStartupAbilities(FantasyASC);
		else
			FantasyASC->AbilitiesGivenDelegate.AddUObject(this, &UOverlayWidgetController::OnInitializeStartupAbilities);

		FantasyASC->EffectAssetTags.AddLambda(
			[this](const FGameplayTagContainer& AssetTags)
			{
				for (const FGameplayTag Tag : AssetTags)
				{
					if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Message"))))
					{
						FUIWidgetRow* Row = GetDataTableRowByTag<FUIWidgetRow>(MessageWidgetDataTable, Tag);
						MessageWidgetRowDelegate.Broadcast(*Row);
					}
				}
			}
		);
	}
}

void UOverlayWidgetController::OnInitializeStartupAbilities(UFantasyAbilitySystemComponent* FantasyASC)
{
	if (!FantasyASC->bStartupAbilitiesGiven) return;

	FForEachAbility BroadcastDelegate;
	BroadcastDelegate.BindLambda([this, FantasyASC](const FGameplayAbilitySpec& AbilitySpec)
	{
		FFantasyAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(FantasyASC->GetAbilityTagFromSpec(AbilitySpec));
		Info.InputTag = FantasyASC->GetInputTagFromSpec(AbilitySpec);
		AbilityInfoDelegate.Broadcast(Info);
	});
	FantasyASC->ForEachAbility(BroadcastDelegate);
}

void UOverlayWidgetController::OnXPChanged(int32 NewXP) const
{
	const AFantasyPlayerState* FantasyPlayerState = CastChecked<AFantasyPlayerState>(PlayerState);
	const ULevelUpInfo* LevelUpInfo = FantasyPlayerState->LevelUpInfo;

	checkf(LevelUpInfo, TEXT("Unabled to find LevelUpInfo, Please fill out FantasyPlayerState Blueprint"));

	const int32 Level = LevelUpInfo->FindLevelForXP(NewXP);
	const int32 MaxLevel = LevelUpInfo->LevelUpInformation.Num();

	if (Level <= MaxLevel && Level > 0)
	{
		const int32 LevelUpRequirement = LevelUpInfo->LevelUpInformation[Level].LevelUpRequirement;
		const int32 PreviousLevelUpRequirement = LevelUpInfo->LevelUpInformation[Level - 1].LevelUpRequirement;
		const int32 DeltaLevelRequirement = LevelUpRequirement - PreviousLevelUpRequirement;
		const int32 XPForThisLevel = NewXP - PreviousLevelUpRequirement;

		const float XPBarPercent = static_cast<float>(XPForThisLevel) / static_cast<float>(DeltaLevelRequirement);

		OnXPPercentChangedDelegate.Broadcast(XPBarPercent);
	}
}
