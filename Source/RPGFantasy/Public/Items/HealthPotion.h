// Copyright Luka Otović

#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "HealthPotion.generated.h"

class USphereComponent;

UCLASS()
class RPGFANTASY_API AHealthPotion : public AItem
{
	GENERATED_BODY()
	
public:	
	AHealthPotion();

protected:
	virtual void BeginPlay() override;
};
