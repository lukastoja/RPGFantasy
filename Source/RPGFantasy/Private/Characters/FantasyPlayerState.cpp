// Copyright Luka Otović


#include "Characters/FantasyPlayerState.h"
#include "AbilitySystem/FantasyAbilitySystemComponent.h"
#include "AbilitySystem/FantasyAttributeSet.h"
#include "Net/UnrealNetwork.h"


AFantasyPlayerState::AFantasyPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UFantasyAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<UFantasyAttributeSet>("AttributeSet");

	NetUpdateFrequency = 100.f;
}

UAbilitySystemComponent* AFantasyPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AFantasyPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AFantasyPlayerState, Level);
	DOREPLIFETIME(AFantasyPlayerState, XP);
}

void AFantasyPlayerState::AddToXP(int32 InXp)
{
	XP += InXp;
	OnXPChangedDelegate.Broadcast(XP);
}

void AFantasyPlayerState::SetXP(int32 InXP)
{
	XP = XP;
	OnXPChangedDelegate.Broadcast(XP);
}

void AFantasyPlayerState::AddToLevel(int32 InLevel)
{
	Level += InLevel;
	OnLevelChangedDelegate.Broadcast(Level);
}

void AFantasyPlayerState::SetLevel(int32 InLevel)
{
	Level = InLevel;
	OnLevelChangedDelegate.Broadcast(Level);
}

void AFantasyPlayerState::OnRep_Level(int32 OldLevel)
{
	OnLevelChangedDelegate.Broadcast(Level);
}

void AFantasyPlayerState::OnRep_XP(int32 OldXP)
{
	OnXPChangedDelegate.Broadcast(XP);
}
