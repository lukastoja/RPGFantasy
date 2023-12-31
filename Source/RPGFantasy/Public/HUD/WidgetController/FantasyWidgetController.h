// Copyright Luka Otović

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "FantasyWidgetController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerStatChangedSignature, int32, NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAbilityInfoSignature, const FFantasyAbilityInfo&, Info);

class UAttributeSet;
class UAbilitySystemComponent;
class AFantasyPlayerController;
class AFantasyPlayerState;
class UFantasyAbilitySystemComponent;
class UFantasyAttributeSet;
class UAbilityInfo;

USTRUCT(BlueprintType)
struct FWidgetControllerParams
{
	GENERATED_BODY()

	FWidgetControllerParams() {}
	FWidgetControllerParams(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
	:  PlayerController (PC), PlayerState(PS), AbilitySystemComponent(ASC), AttributeSet(AS) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TObjectPtr<APlayerController> PlayerController = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TObjectPtr<APlayerState> PlayerState = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TObjectPtr<UAttributeSet> AttributeSet = nullptr;
};

UCLASS()
class RPGFANTASY_API UFantasyWidgetController : public UObject
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
		void SetWidgetControllerParams(const FWidgetControllerParams& WCParams);

	UFUNCTION(BlueprintCallable)
		virtual void BroadcastInitialValues();

	virtual void BindCallbacksToDependencies();

	void BroadcastAbilityInfo();

	UPROPERTY(BlueprintAssignable, Category = "GAS|Messages")
		FAbilityInfoSignature AbilityInfoDelegate;

protected:

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
		TObjectPtr<APlayerController> PlayerController;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
		TObjectPtr<APlayerState> PlayerState;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
		TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
		TObjectPtr<UAttributeSet> AttributeSet;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
		TObjectPtr<AFantasyPlayerController> FantasyPlayerController;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
		TObjectPtr<AFantasyPlayerState> FantasyPlayerState;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
		TObjectPtr<UFantasyAbilitySystemComponent> FantasyAbilitySystemComponent;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
		TObjectPtr<UFantasyAttributeSet> FantasyAttributeSet;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Widget Data")
		TObjectPtr<UAbilityInfo> AbilityInfo;

	AFantasyPlayerController* GetFantasyPC();
	AFantasyPlayerState* GetFantasyPS();
	UFantasyAbilitySystemComponent* GetFantasyASC();
	UFantasyAttributeSet* GetFantasyAS();
};
