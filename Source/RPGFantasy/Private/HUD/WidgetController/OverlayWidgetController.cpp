// Copyright Luka Otović


#include "HUD/WidgetController/OverlayWidgetController.h"
#include "AbilitySystem/FantasyAttributeSet.h"
#include "AbilitySystem/FantasyAbilitySystemComponent.h"

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
