// Copyright Luka Otović


#include "HUD/ViewModel/MVVM_LoadScreen.h"
#include "HUD/ViewModel/MVVM_LoadSlot.h"
#include <Game/FantasyGameModeBase.h>
#include <Kismet/GameplayStatics.h>

void UMVVM_LoadScreen::InitializeLoadSlots()
{
	LoadSlot_0 = NewObject<UMVVM_LoadSlot>(this, LoadSlotViewModelClass);
	LoadSlot_0->LoadSlotName = FString("LoadSlot_0");
	LoadSlot_0->SlotIndex = 0;

	LoadSlot_1 = NewObject<UMVVM_LoadSlot>(this, LoadSlotViewModelClass);
	LoadSlot_1->LoadSlotName = FString("LoadSlot_1");
	LoadSlot_0->SlotIndex = 1;

	LoadSlot_2 = NewObject<UMVVM_LoadSlot>(this, LoadSlotViewModelClass);
	LoadSlot_2->LoadSlotName = FString("LoadSlot_2");
	LoadSlot_0->SlotIndex = 2;

	LoadSlots.Add(0, LoadSlot_0);
	LoadSlots.Add(1, LoadSlot_1);
	LoadSlots.Add(2, LoadSlot_2);
}

void UMVVM_LoadScreen::LoadData()
{
	AFantasyGameModeBase* FantasyGameMode = Cast<AFantasyGameModeBase>(UGameplayStatics::GetGameMode(this));

	for (const TTuple<int32, UMVVM_LoadSlot*> LoadSlot : LoadSlots)
	{
		ULoadScreenSaveGame* SaveObject = FantasyGameMode->GetSaveSlotData(LoadSlot.Value->LoadSlotName, LoadSlot.Key);

		const FString PlayerName = SaveObject->PlayerName;
		const FString MapName = SaveObject->MapName;

		TEnumAsByte<ESaveSlotStatus> SaveSlotStatus = SaveObject->SaveSlotStatus;

		LoadSlot.Value->SlotStatus = SaveSlotStatus;
		LoadSlot.Value->SetPlayerName(PlayerName);
		LoadSlot.Value->SetMapName(MapName);
		LoadSlot.Value->InitializeSlot();
	}
}

void UMVVM_LoadScreen::NewSlotButtonPressed(int32 Slot, const FString& EnteredName)
{
	AFantasyGameModeBase* FantasyGameMode = Cast<AFantasyGameModeBase>(UGameplayStatics::GetGameMode(this));

	LoadSlots[Slot]->SetMapName(FantasyGameMode->DefaultMapName);
	LoadSlots[Slot]->SetPlayerName(EnteredName);
	LoadSlots[Slot]->SlotStatus = Taken;

	FantasyGameMode->SaveSlotData(LoadSlots[Slot], Slot);
	LoadSlots[Slot]->InitializeSlot();
}

void UMVVM_LoadScreen::NewGameButtonPressed(int32 Slot)
{
	LoadSlots[Slot]->SetWidgetSwitcherIndex.Broadcast(1);
}

void UMVVM_LoadScreen::SelectSlotButtonPressed(int32 Slot)
{
	SlotSelected.Broadcast();

	for (const TTuple<int32, UMVVM_LoadSlot*> LoadSlot : LoadSlots)
	{
		if(LoadSlot.Key == Slot)
			LoadSlot.Value->EnableSelectSlotButton.Broadcast(false);
		else
			LoadSlot.Value->EnableSelectSlotButton.Broadcast(true);
	}
	SelectedSlot = LoadSlots[Slot];
}

void UMVVM_LoadScreen::DeleteButtonPressed()
{
	if (IsValid(SelectedSlot))
	{
		AFantasyGameModeBase::DeleteSlot(SelectedSlot->LoadSlotName, SelectedSlot->SlotIndex);
		SelectedSlot->SlotStatus = Vacant;
		SelectedSlot->InitializeSlot();
		SelectedSlot->EnableSelectSlotButton.Broadcast(true);
	}
}

void UMVVM_LoadScreen::PlayButtonPressed()
{
	AFantasyGameModeBase* FantasyGameMode = Cast<AFantasyGameModeBase>(UGameplayStatics::GetGameMode(this));
	if (IsValid(SelectedSlot))
		FantasyGameMode->TravelToMap(SelectedSlot);
}

UMVVM_LoadSlot* UMVVM_LoadScreen::GetLoadSlotViewModelByIndex(int32 Index) const
{
	return LoadSlots.FindChecked(Index);
}
