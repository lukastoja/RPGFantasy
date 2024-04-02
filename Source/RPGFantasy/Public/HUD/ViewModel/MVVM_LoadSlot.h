// Copyright Luka Otović

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "Game/LoadScreenSaveGame.h"
#include "MVVM_LoadSlot.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSetWidgetSwitcherIndex, int32, WidgetSwitcherIndex);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEnableSelectSlotButton, bool, bEnable);

UCLASS(Blueprintable)
class RPGFANTASY_API UMVVM_LoadSlot : public UMVVMViewModelBase
{
	GENERATED_BODY()

public:

	void InitializeSlot();

	void SetPlayerName(FString InPlayerName);

	FString GetPlayerName() const { return PlayerName; }

	void SetMapName(FString InMapName);

	FString GetMapName() const { return MapName; }

	UPROPERTY(BlueprintAssignable)
		FSetWidgetSwitcherIndex SetWidgetSwitcherIndex;

	UPROPERTY(BlueprintAssignable)
		FEnableSelectSlotButton EnableSelectSlotButton;

	UPROPERTY()
		FString LoadSlotName;

	UPROPERTY()
		int32 SlotIndex;

	UPROPERTY()
		TEnumAsByte<ESaveSlotStatus> SlotStatus;

private:

	//Field Notifies
	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta = (AllowPrivateAccess="true"))
		FString PlayerName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta = (AllowPrivateAccess = "true"))
		FString MapName;
};
