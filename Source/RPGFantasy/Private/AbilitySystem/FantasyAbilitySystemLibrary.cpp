// Copyright Luka Otović


#include "AbilitySystem/FantasyAbilitySystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "HUD/FantasyHUD.h"
#include "Characters/FantasyPlayerState.h"
#include "HUD/WidgetController/AttributeMenuWidgetController.h"
#include "Game/FantasyGameModeBase.h"
#include "AbilitySystemComponent.h"
#include "FantasyAbilityTypes.h"
#include "Characters/BaseCharacter.h"
#include "HUD/WidgetController/FantasyWidgetController.h"
#include <AbilitySystemBlueprintLibrary.h>
#include <FantasyGameplayTags.h>


bool UFantasyAbilitySystemLibrary::MakeWidgetControllerParams(const UObject* WorldContextObject, FWidgetControllerParams& OutWCParams, AFantasyHUD*& OutFantasyHud)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		OutFantasyHud = Cast<AFantasyHUD>(PC->GetHUD());
		if (OutFantasyHud)
		{
			AFantasyPlayerState* PS = PC->GetPlayerState<AFantasyPlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();
			OutWCParams.AttributeSet = AS;
			OutWCParams.AbilitySystemComponent = ASC;
			OutWCParams.PlayerController = PC;
			OutWCParams.PlayerState = PS;
			return true;
		}
	}
	return false;
}

UOverlayWidgetController* UFantasyAbilitySystemLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{

	FWidgetControllerParams WCParams;
	AFantasyHUD* FantasyHUD = nullptr;

	if (MakeWidgetControllerParams(WorldContextObject, WCParams, FantasyHUD))
		return FantasyHUD->GetOverlayWidgetController(WCParams);

	return nullptr;
}

UAttributeMenuWidgetController* UFantasyAbilitySystemLibrary::GetAttributeMenuWidgetController(const UObject* WorldContextObject)
{
	FWidgetControllerParams WCParams;
	AFantasyHUD* FantasyHUD = nullptr;

	if (MakeWidgetControllerParams(WorldContextObject, WCParams, FantasyHUD))
		return FantasyHUD->GetAttributeMenuWidgetController(WCParams);

	return nullptr;
}

USpellMenuWidgetController* UFantasyAbilitySystemLibrary::GetSpellMenuWidgetController(const UObject* WorldContextObject)
{
	FWidgetControllerParams WCParams;
	AFantasyHUD* FantasyHUD = nullptr;

	if (MakeWidgetControllerParams(WorldContextObject, WCParams, FantasyHUD))
		return FantasyHUD->GetSpellMenuWidgetController(WCParams);

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


void UFantasyAbilitySystemLibrary::GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC, ECharacterClass CharacterClass)
{
	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	if (CharacterClassInfo == nullptr) return;

	for (TSubclassOf<UGameplayAbility> AbilityClass : CharacterClassInfo->CommonAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		ASC->GiveAbility(AbilitySpec);
	}
	const FCharacterClassDefaultInfo& DefaultInfo = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);
	for (TSubclassOf<UGameplayAbility> AbilityClass : DefaultInfo.StartupAbilities)
	{
		if (ASC->GetAvatarActor()->Implements<UCombatInterface>())
		{
			FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, ICombatInterface::Execute_GetPlayerLevel(ASC->GetAvatarActor()));
			ASC->GiveAbility(AbilitySpec);
		}
	}
}

int32 UFantasyAbilitySystemLibrary::GetXPRewardForClassAndLevel(ECharacterClass CharacterClass, int32 CharacterLevel, const UObject* WorldContextObject)
{
	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	if (CharacterClassInfo == nullptr) return 0;

	const FCharacterClassDefaultInfo& Info = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);

	const float XPReward = Info.XPReward.GetValueAtLevel(CharacterLevel);

	return static_cast<int32>(XPReward);
}

UCharacterClassInfo* UFantasyAbilitySystemLibrary::GetCharacterClassInfo(const UObject* WorldContextObject)
{
	const AFantasyGameModeBase* FantasyGameMode = Cast<AFantasyGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (FantasyGameMode == nullptr) return nullptr;

	return FantasyGameMode->CharacterClassInfo;
}

UAbilityInfo* UFantasyAbilitySystemLibrary::GetAbilityInfo(const UObject* WorldContextObject)
{
	const AFantasyGameModeBase* FantasyGameMode = Cast<AFantasyGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (FantasyGameMode == nullptr) return nullptr;

	return FantasyGameMode->AbilityInfo;
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

bool UFantasyAbilitySystemLibrary::IsSuccessfulDebuff(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FFantasyGameplayEffectContext* FantasyContext = static_cast<const FFantasyGameplayEffectContext*>(EffectContextHandle.Get()))
		return FantasyContext->IsSuccessfulDebuff();
	return false;
}

float UFantasyAbilitySystemLibrary::GetDebuffDamage(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FFantasyGameplayEffectContext* FantasyContext = static_cast<const FFantasyGameplayEffectContext*>(EffectContextHandle.Get()))
		return FantasyContext->GetDebuffDamage();
	return 0.f;
}

float UFantasyAbilitySystemLibrary::GetDebuffDuration(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FFantasyGameplayEffectContext* FantasyContext = static_cast<const FFantasyGameplayEffectContext*>(EffectContextHandle.Get()))
		return FantasyContext->GetDebuffDuration();
	return 0.f;
}

float UFantasyAbilitySystemLibrary::GetDebuffFrequency(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FFantasyGameplayEffectContext* FantasyContext = static_cast<const FFantasyGameplayEffectContext*>(EffectContextHandle.Get()))
		return FantasyContext->GetDebuffFrequency();
	return 0.f;
}

FGameplayTag UFantasyAbilitySystemLibrary::GetDamageType(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FFantasyGameplayEffectContext* FantasyContext = static_cast<const FFantasyGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		if (FantasyContext->GetDamageType().IsValid())
			return *FantasyContext->GetDamageType();
	}
	return FGameplayTag();
}

FVector UFantasyAbilitySystemLibrary::GetDeathImpulse(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FFantasyGameplayEffectContext* FantasyContext = static_cast<const FFantasyGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return FantasyContext->GetDeathImpulse();
	}
	return FVector::ZeroVector;
}

FVector UFantasyAbilitySystemLibrary::GetKonckbackForce(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FFantasyGameplayEffectContext* FantasyContext = static_cast<const FFantasyGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return FantasyContext->GetKnockbackForce();
	}
	return FVector::ZeroVector;
}

void UFantasyAbilitySystemLibrary::SetIsSuccessfulDebuff(UPARAM(ref)FGameplayEffectContextHandle& EffectContextHandle, bool bInIsSuccessfulDebuff)
{
	if (FFantasyGameplayEffectContext* FantasyEffectContext = static_cast<FFantasyGameplayEffectContext*>(EffectContextHandle.Get()))
		FantasyEffectContext->SetIsSuccessfulDebuff(bInIsSuccessfulDebuff);
}

void UFantasyAbilitySystemLibrary::SetDebuffDamage(UPARAM(ref)FGameplayEffectContextHandle& EffectContextHandle, float InDamage)
{
	if (FFantasyGameplayEffectContext* FantasyEffectContext = static_cast<FFantasyGameplayEffectContext*>(EffectContextHandle.Get()))
		FantasyEffectContext->SetDebuffDamage(InDamage);
}

void UFantasyAbilitySystemLibrary::SetDebuffDuration(UPARAM(ref)FGameplayEffectContextHandle& EffectContextHandle, float InDuration)
{
	if (FFantasyGameplayEffectContext* FantasyEffectContext = static_cast<FFantasyGameplayEffectContext*>(EffectContextHandle.Get()))
		FantasyEffectContext->SetDebuffDuration(InDuration);
}

void UFantasyAbilitySystemLibrary::SetDebuffFrequency(UPARAM(ref)FGameplayEffectContextHandle& EffectContextHandle, float InFrequency)
{
	if (FFantasyGameplayEffectContext* FantasyEffectContext = static_cast<FFantasyGameplayEffectContext*>(EffectContextHandle.Get()))
		FantasyEffectContext->SetDebuffFrequency(InFrequency);
}

void UFantasyAbilitySystemLibrary::SetDamageType(UPARAM(ref)FGameplayEffectContextHandle& EffectContextHandle, const FGameplayTag& InDamageType)
{
	if (FFantasyGameplayEffectContext* FantasyEffectContext = static_cast<FFantasyGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		const TSharedPtr<FGameplayTag> DamageType = MakeShared<FGameplayTag>(InDamageType);
		FantasyEffectContext->SetDamageType(DamageType);
	}
}

void UFantasyAbilitySystemLibrary::SetDeathImpulse(UPARAM(ref)FGameplayEffectContextHandle& EffectContextHandle, const FVector& InDeathImpulse)
{
	if (FFantasyGameplayEffectContext* FantasyEffectContext = static_cast<FFantasyGameplayEffectContext*>(EffectContextHandle.Get()))
		FantasyEffectContext->SetDeathImpulse(InDeathImpulse);
}

void UFantasyAbilitySystemLibrary::SetKonckbackForce(UPARAM(ref)FGameplayEffectContextHandle& EffectContextHandle, const FVector& InForce)
{
	if (FFantasyGameplayEffectContext* FantasyEffectContext = static_cast<FFantasyGameplayEffectContext*>(EffectContextHandle.Get()))
		FantasyEffectContext->SetKonckbackForce(InForce);
}

void UFantasyAbilitySystemLibrary::GetLivePlayersWithinRadius(const UObject* WorldContextObject, TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorsToIgnore, float Radius, const FVector& SphereOrigin)
{
	FCollisionQueryParams SphereParams;
	SphereParams.AddIgnoredActors(ActorsToIgnore);

	TArray<FOverlapResult> Overlaps;
	if (const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		World->OverlapMultiByObjectType(Overlaps, SphereOrigin, FQuat::Identity, FCollisionObjectQueryParams(FCollisionObjectQueryParams::InitType::AllDynamicObjects), FCollisionShape::MakeSphere(Radius), SphereParams);
		for (FOverlapResult& Overlap : Overlaps)
		{
			if (Overlap.GetActor()->Implements<UCombatInterface>() && !ICombatInterface::Execute_IsDead(Overlap.GetActor()))
				OutOverlappingActors.AddUnique(ICombatInterface::Execute_GetAvatar(Overlap.GetActor()));
		}
	}
}

bool UFantasyAbilitySystemLibrary::IsNotFriend(AActor* FirstActor, AActor* SecondActor)
{
	const bool bBothArePlayers = FirstActor->ActorHasTag(FName("Player")) && SecondActor->ActorHasTag(FName("Player"));
	const bool bBothAreEnemies = FirstActor->ActorHasTag(FName("Enemy")) && SecondActor->ActorHasTag(FName("Enemy"));
	const bool bFriends = bBothArePlayers || bBothAreEnemies;
	return !bFriends;
}

FGameplayEffectContextHandle UFantasyAbilitySystemLibrary::ApplyDamageEffect(const FDamageEffectParams& DamageEffectParams)
{
	const FFantasyGameplayTags& GameplayTags = FFantasyGameplayTags::Get();
	const AActor* SourceAvatarActor = DamageEffectParams.SourceAbilitySystemComponent->GetAvatarActor();
	
	FGameplayEffectContextHandle EffectContextHandle =  DamageEffectParams.SourceAbilitySystemComponent->MakeEffectContext();
	EffectContextHandle.AddSourceObject(SourceAvatarActor);
	SetDeathImpulse(EffectContextHandle, DamageEffectParams.DeathImpulse);
	SetKonckbackForce(EffectContextHandle, DamageEffectParams.KonckbackForce);

	const FGameplayEffectSpecHandle SpecHandle = DamageEffectParams.SourceAbilitySystemComponent->MakeOutgoingSpec(DamageEffectParams.DamageGameplayEffectClass, DamageEffectParams.AbilityLevel, EffectContextHandle);

	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, DamageEffectParams.DamageType, DamageEffectParams.BaseDamage);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Debuff_Chance, DamageEffectParams.DebuffChance);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Debuff_Damage, DamageEffectParams.DebuffDamage);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Debuff_Duration, DamageEffectParams.DebuffDuration);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Debuff_Frequency, DamageEffectParams.DebuffFrequency);

	DamageEffectParams.TargetAbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data);

	return EffectContextHandle;
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
