// Copyright Luka Otović

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "MVVM_LoadScreen.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSlotSelected);

class UMVVM_LoadSlot;

UCLASS(Blueprintable)
class RPGFANTASY_API UMVVM_LoadScreen : public UMVVMViewModelBase
{
	GENERATED_BODY()

public:

	void InitializeLoadSlots();
	void LoadData();

	UPROPERTY(BlueprintAssignable)
		FSlotSelected SlotSelected;

	UFUNCTION(BlueprintCallable)
		void NewSlotButtonPressed(int32 Slot, const FString& EnteredName);

	UFUNCTION(BlueprintCallable)
		void NewGameButtonPressed(int32 Slot);

	UFUNCTION(BlueprintCallable)
		void SelectSlotButtonPressed(int32 Slot);

	UFUNCTION(BlueprintCallable)
		void DeleteButtonPressed();

	UFUNCTION(BlueprintCallable)
		void PlayButtonPressed();

	UFUNCTION(BlueprintPure)
		UMVVM_LoadSlot* GetLoadSlotViewModelByIndex(int32 Index) const;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<UMVVM_LoadSlot> LoadSlotViewModelClass;

private:
	UPROPERTY()
		TMap<int32, UMVVM_LoadSlot*> LoadSlots;

	UPROPERTY()
		TObjectPtr<UMVVM_LoadSlot> LoadSlot_0;

	UPROPERTY()
		TObjectPtr<UMVVM_LoadSlot> LoadSlot_1;

	UPROPERTY()
		TObjectPtr<UMVVM_LoadSlot> LoadSlot_2;

	UPROPERTY()
		TObjectPtr<UMVVM_LoadSlot> SelectedSlot;
};
