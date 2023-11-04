// Copyright Luka Otović


#include "AbilitySystem/FantasyAbilitySystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "HUD/FantasyHUD.h"
#include "Characters/FantasyPlayerState.h"
#include "HUD/WidgetController/FantasyWidgetController.h"
#include "HUD/WidgetController/AttributeMenuWidgetController.h"
#include "Game/FantasyGameModeBase.h"
#include "AbilitySystemComponent.h"
#include "FantasyAbilityTypes.h"


UOverlayWidgetController* UFantasyAbilitySystemLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (AFantasyHUD* FantasyHUD = Cast<AFantasyHUD>(PC->GetHUD()))
		{
			AFantasyPlayerState* PS = PC->GetPlayerState<AFantasyPlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();
			const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
			return FantasyHUD->GetOverlayWidgetController(WidgetControllerParams);
		}
	}
	return nullptr;
}

UAttributeMenuWidgetController* UFantasyAbilitySystemLibrary::GetAttributeMenuWidgetController(const UObject* WorldContextObject)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (AFantasyHUD* FantasyHUD = Cast<AFantasyHUD>(PC->GetHUD()))
		{
			AFantasyPlayerState* PS = PC->GetPlayerState<AFantasyPlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();
			const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
			return FantasyHUD->GetAttributeMenuWidgetController(WidgetControllerParams);
		}
	}
	return nullptr;
}

void UFantasyAbilitySystemLibrary::InitializeDefaultAttributes(const UObject* WorldContextObject, ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* ASC)
{
	AActor* AvatarActor = ASC->GetAvatarActor();

	FCharacterClassDefaultInfo ClassDefaultInfo = GetCharacterClassInfo(WorldContextObject)->GetClassDefaultInfo(CharacterClass);

	FGameplayEffectContextHandle PrimaryAttributesContextHandle = ASC->MakeEffectContext();
	PrimaryAttributesContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle PrimaryAttributesSpechandle = ASC->MakeOutgoingSpec(ClassDefaultInfo.PrimaryAttributes, Level, PrimaryAttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*PrimaryAttributesSpechandle.Data.Get());


	FGameplayEffectContextHandle SecondaryAttributesContextHandle = ASC->MakeEffectContext();
	SecondaryAttributesContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle SecondaryAttributesSpechandle = ASC->MakeOutgoingSpec(GetCharacterClassInfo(WorldContextObject)->SecondaryAttributes, Level, SecondaryAttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*SecondaryAttributesSpechandle.Data.Get());


	FGameplayEffectContextHandle VitalAttributesContextHandle = ASC->MakeEffectContext();
	VitalAttributesContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle VitalAttributesSpechandle = ASC->MakeOutgoingSpec(GetCharacterClassInfo(WorldContextObject)->VitalAttributes, Level, VitalAttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*VitalAttributesSpechandle.Data.Get());
}


void UFantasyAbilitySystemLibrary::GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC)
{

	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	for (TSubclassOf<UGameplayAbility> AbilityClass : CharacterClassInfo->CommonAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		ASC->GiveAbility(AbilitySpec);
	}
}

UCharacterClassInfo* UFantasyAbilitySystemLibrary::GetCharacterClassInfo(const UObject* WorldContextObject)
{
	AFantasyGameModeBase* FantasyGameMode = Cast<AFantasyGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (FantasyGameMode == nullptr) return nullptr;

	return FantasyGameMode->CharacterClassInfo;
}

bool UFantasyAbilitySystemLibrary::IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FFantasyGameplayEffectContext* FantasyContext = static_cast<const FFantasyGameplayEffectContext*>(EffectContextHandle.Get()))
		return FantasyContext->IsBlockedHit();
	return false;
}

bool UFantasyAbilitySystemLibrary::IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FFantasyGameplayEffectContext* FantasyContext = static_cast<const FFantasyGameplayEffectContext*>(EffectContextHandle.Get()))
		return FantasyContext->IsCriticalHit();
	return false;
}

void UFantasyAbilitySystemLibrary::SetIsBlockedHit(FGameplayEffectContextHandle& EffectContextHandle, bool bInIsBlockedHit)
{
	if (FFantasyGameplayEffectContext* FantasyEffectContext = static_cast<FFantasyGameplayEffectContext*>(EffectContextHandle.Get()))
		FantasyEffectContext->SetIsBlockedHit(bInIsBlockedHit);
}

void UFantasyAbilitySystemLibrary::SetIsCriticalHit(FGameplayEffectContextHandle& EffectContextHandle, bool bInIsCriticalHit)
{
	if (FFantasyGameplayEffectContext* FantasyEffectContext = static_cast<FFantasyGameplayEffectContext*>(EffectContextHandle.Get()))
		FantasyEffectContext->SetIsCriticalHit(bInIsCriticalHit);
}
