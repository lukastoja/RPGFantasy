// Copyright Luka Otović

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "FantasyAIController.generated.h"

class UBlackboardComponent;
class UBehaviorTreeComponent;

UCLASS()
class RPGFANTASY_API AFantasyAIController : public AAIController
{
	GENERATED_BODY()

public:
	AFantasyAIController();

protected:

	UPROPERTY()
		TObjectPtr<UBehaviorTreeComponent> BehaviorTreeComponent;
};
