// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/AuraGameMode.h"

#include "Game/LoadScreenSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "UI/ViewModel/MVVM_LoadSlot.h"

void AAuraGameMode::BeginPlay()
{
	Super::BeginPlay();
	Maps.Add(StartLoadMapName,DefaultMap);
}


void AAuraGameMode::SaveSlotData(UMVVM_LoadSlot* SaveSlot, int32 SlotIndex)
{
	DeleteSlot(SaveSlot->GetLoadSlotName(),SlotIndex);
	USaveGame* SaveGameObject =	UGameplayStatics::CreateSaveGameObject(LoadScreenSaveGameClass);
	ULoadScreenSaveGame* LoadScreenSaveGame = Cast<ULoadScreenSaveGame>(SaveGameObject);
	LoadScreenSaveGame->PlayerName = SaveSlot->GetPlayerName();
	LoadScreenSaveGame->SlotStatus = SaveSlot->SlotStatus;
	LoadScreenSaveGame->CurMapName = SaveSlot->GetMapName();
	
	UGameplayStatics::SaveGameToSlot(LoadScreenSaveGame,SaveSlot->GetLoadSlotName(),SlotIndex);
}

ULoadScreenSaveGame* AAuraGameMode::GetSaveGameSlot(const FString& SlotName, int32 SlotIndex)
{
	USaveGame* CurrentSaveGameObject = nullptr;
	if (UGameplayStatics::DoesSaveGameExist(SlotName,SlotIndex))
	{
		CurrentSaveGameObject = UGameplayStatics::LoadGameFromSlot(SlotName,SlotIndex);
	}
	else
	{
		CurrentSaveGameObject = UGameplayStatics::CreateSaveGameObject(LoadScreenSaveGameClass);
	}
	
	ULoadScreenSaveGame* LoadScreenSaveGame = Cast<ULoadScreenSaveGame>(CurrentSaveGameObject);
	return LoadScreenSaveGame;
	
}

void AAuraGameMode::TravelToMap(UMVVM_LoadSlot* SaveSlot)
{
	
	UGameplayStatics::OpenLevelBySoftObjectPtr(SaveSlot,Maps.FindChecked(SaveSlot->GetMapName()));
}

void AAuraGameMode::DeleteSlot(const FString& SlotName, int32 SlotIndex)
{
	if (UGameplayStatics::DoesSaveGameExist(SlotName,SlotIndex))
	{
		UGameplayStatics::DeleteGameInSlot(SlotName,SlotIndex);
	}
}

