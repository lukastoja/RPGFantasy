// Copyright Luka Otović

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameplayTagContainer.h"
#include "FantasyPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class UFantasyInputConfig;
class UFantasyAbilitySystemComponent;

UCLASS()
class RPGFANTASY_API AFantasyPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AFantasyPlayerController();

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

private:
	UPROPERTY(EditAnywhere, Category = "Input")
		TObjectPtr<UInputMappingContext> FantasyContext;

	UPROPERTY(EditAnywhere, Category = "Input")
		TObjectPtr<UInputAction> MoveAction;

	void Move(const FInputActionValue& InputActionValue);
	
	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);


	UPROPERTY(EditDefaultsOnly, Category = "Input")
		TObjectPtr<UFantasyInputConfig> InputConfig;

	UPROPERTY()
		TObjectPtr<UFantasyAbilitySystemComponent> FantasyAbilitySystemComponent;

	UFantasyAbilitySystemComponent* GetASC();
};
