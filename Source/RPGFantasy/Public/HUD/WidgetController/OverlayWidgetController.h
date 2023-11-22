// Copyright Luka Otović

#pragma once

#include "CoreMinimal.h"
#include "HUD/WidgetController/FantasyWidgetController.h"
#include "GameplayEffectTypes.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "OverlayWidgetController.generated.h"

class UFantasyUserWidget;
class UAbilityInfo;
class UFantasyAbilitySystemComponent;

USTRUCT(BlueprintType)
struct FUIWidgetRow : public FTableRowBase
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FGameplayTag MessageTag = FGameplayTag();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FText Message = FText();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TSubclassOf<UFantasyUserWidget> MessageWidget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		UTexture2D* Image = nullptr;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerStatChangedSignature, int32, NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeChangedSignature, float, NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMessageWidgetRowSignature, FUIWidgetRow, Row);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAbilityInfoSignature, const FFantasyAbilityInfo&, Info);

UCLASS(BlueprintType, Blueprintable)
class RPGFANTASY_API UOverlayWidgetController : public UFantasyWidgetController
{
	GENERATED_BODY()

public:
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attribute")
		FOnAttributeChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attribute")
		FOnAttributeChangedSignature OnMaxHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attribute")
		FOnAttributeChangedSignature OnManaChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attribute")
		FOnAttributeChangedSignature OnMaxManaChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Messages")
		FMessageWidgetRowSignature MessageWidgetRowDelegate;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Messages")
		FAbilityInfoSignature AbilityInfoDelegate;

	UPROPERTY(BlueprintAssignable, Category = "GAS|XP")
		FOnAttributeChangedSignature OnXPPercentChangedDelegate;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Level")
		FOnPlayerStatChangedSignature OnPlayerLevelChangedDelegate;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Widget Data")
		TObjectPtr<UDataTable> MessageWidgetDataTable;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Widget Data")
		TObjectPtr<UAbilityInfo> AbilityInfo;

	template<typename T>
	T* GetDataTableRowByTag(UDataTable* DataTable, const FGameplayTag& Tag);

	void OnInitializeStartupAbilities(UFantasyAbilitySystemComponent* FantasyASC);

	void OnXPChanged(int32 NewXP) const;
};

template<typename T>
inline T* UOverlayWidgetController::GetDataTableRowByTag(UDataTable* DataTable, const FGameplayTag& Tag)
{
	return DataTable->FindRow<T>(Tag.GetTagName(), TEXT(""));
}
