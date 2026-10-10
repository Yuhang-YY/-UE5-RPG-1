// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ViewModel/MVVM_LoadScreen.h"

#include "Game/AuraGameMode.h"
#include "Game/LoadScreenSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "UI/ViewModel/MVVM_LoadSlot.h"

UMVVM_LoadSlot* UMVVM_LoadScreen::GetLoadSlotViewModel(int32 Index)
{
	return LoadSlotViewModelMap.FindChecked(Index);
}

void UMVVM_LoadScreen::InitializeLoadSlotViewModel()
{
	LoadSlot_0 = NewObject<UMVVM_LoadSlot>(this,LoadSlotViewModelClass);
	LoadSlot_0->SetLoadSlotName("LoadSlot_0");
	LoadSlot_0->LoadSlotIndex = 0;
	LoadSlotViewModelMap.Add(0,LoadSlot_0);
	
	LoadSlot_1 = NewObject<UMVVM_LoadSlot>(this,LoadSlotViewModelClass);
	LoadSlot_1->SetLoadSlotName("LoadSlot_1");
	LoadSlot_1->LoadSlotIndex = 1;
	LoadSlotViewModelMap.Add(1,LoadSlot_1);
	
	LoadSlot_2 = NewObject<UMVVM_LoadSlot>(this,LoadSlotViewModelClass);
	LoadSlot_2->SetLoadSlotName("LoadSlot_2");
	LoadSlot_2->LoadSlotIndex = 2;
	LoadSlotViewModelMap.Add(2,LoadSlot_2);
}

void UMVVM_LoadScreen::NewSlotButtonPressed(int32 Index, const FString& PlayerName)
{
	AAuraGameMode* AuraGameMode = Cast<AAuraGameMode>(UGameplayStatics::GetGameMode(this));
	if (AuraGameMode)
	{
		LoadSlotViewModelMap[Index]->SetPlayerName(PlayerName);
		LoadSlotViewModelMap[Index]->SlotStatus = Taken;
		LoadSlotViewModelMap[Index]->SetMapName(AuraGameMode->StartLoadMapName);
		
		AuraGameMode->SaveSlotData(LoadSlotViewModelMap[Index],Index);
		LoadSlotViewModelMap[Index]->InitializeSlot();
	}
}

void UMVVM_LoadScreen::NewGameButtonPressed(int32 Index)
{
	LoadSlotViewModelMap[Index]->SetWidgetSwitcherIndex.Broadcast(1);
}

void UMVVM_LoadScreen::SelectionSlotButtonPressed(int32 Index)
{
	SelectedButton.Broadcast();
	for (TTuple<int32,UMVVM_LoadSlot*> LoadSlot : LoadSlotViewModelMap)
	{
		if (LoadSlot.Key == Index)
		{
			LoadSlot.Value->EnableSelectedSlotButton.Broadcast(false);
		}
		else
		{
			LoadSlot.Value->EnableSelectedSlotButton.Broadcast(true);
		}
	}
	
	SelectedSlot = LoadSlotViewModelMap[Index];
}

void UMVVM_LoadScreen::SureDeleteButtonPressed()
{
	AAuraGameMode::DeleteSlot(SelectedSlot->GetLoadSlotName(),SelectedSlot->LoadSlotIndex);
	SelectedSlot->SlotStatus = Vacant;
	SelectedSlot->EnableSelectedSlotButton.Broadcast(true);
	SelectedSlot->InitializeSlot();
}

void UMVVM_LoadScreen::PlayButtonPressed()
{
	AAuraGameMode* AuraGameMode = Cast<AAuraGameMode>(UGameplayStatics::GetGameMode(this));
	AuraGameMode->TravelToMap(SelectedSlot);
}

void UMVVM_LoadScreen::LoadData()
{
	AAuraGameMode* AuraGameMode = Cast<AAuraGameMode>(UGameplayStatics::GetGameMode(this));
	if (!AuraGameMode) return;
	
	for (TTuple<int32,UMVVM_LoadSlot*> LoadSlot : LoadSlotViewModelMap)
	{
		ULoadScreenSaveGame* SaveGameObject = AuraGameMode->GetSaveGameSlot(LoadSlot.Value->GetLoadSlotName(),LoadSlot.Key);
		TEnumAsByte<ESaveSlotStatus> SaveSlotStatus = SaveGameObject->SlotStatus;
		FString PlayerName = SaveGameObject->PlayerName;
		FString MapName = SaveGameObject->CurMapName;
		
		
		LoadSlot.Value->SlotStatus = SaveSlotStatus;
		LoadSlot.Value->SetPlayerName(PlayerName);
		LoadSlot.Value->SetMapName(MapName);
		LoadSlot.Value->InitializeSlot();
	}
}
