// Copyright Luka Otović


#include "Items/HealthPotion.h"
#include "Components/Spherecomponent.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/FantasyAttributeSet.h"

//fucking potion prebaciti ovo smece u da nasljeđuje item?
AHealthPotion::AHealthPotion()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AHealthPotion::BeginPlay()
{
	Super::BeginPlay();
}
