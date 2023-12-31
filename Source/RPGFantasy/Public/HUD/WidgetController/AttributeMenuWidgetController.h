// Copyright Luka Otović

#pragma once

#include "CoreMinimal.h"
#include "HUD/WidgetController/FantasyWidgetController.h"
#include "AbilitySystem/Data/AttributeInfo.h"
#include "GameplayTagContainer.h"
#include "AttributeSet.h"
#include "AttributeMenuWidgetController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAttributeInfoSignature, const FFantasyAttributeInfo&, Info);

UCLASS(BlueprintType, Blueprintable)
class RPGFANTASY_API UAttributeMenuWidgetController : public UFantasyWidgetController
{
	GENERATED_BODY()
	
public:
	virtual void BindCallbacksToDependencies() override;
	virtual void BroadcastInitialValues() override;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
		FAttributeInfoSignature AttributeInfoDelegate;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
		FOnPlayerStatChangedSignature AttributePointsChangedDelegate;

	UFUNCTION(Blueprintcallable)
		void UpgradeAttribute(const FGameplayTag& AttributeTag);

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		TObjectPtr<UAttributeInfo> AttributeInfo;

private:

	void BroadcastAttributeInfo(const FGameplayTag& AttributeTag, const FGameplayAttribute& Attribute) const;
};
