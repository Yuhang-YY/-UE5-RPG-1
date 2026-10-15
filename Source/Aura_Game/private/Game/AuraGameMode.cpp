// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/AuraGameMode.h"

#include "Game/AuraGameInstance.h"
#include "Game/LoadScreenSaveGame.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "UI/ViewModel/MVVM_LoadSlot.h"

void AAuraGameMode::BeginPlay()
{
	Super::BeginPlay();
	Maps.Add(StartLoadMapName,DefaultMap);
}

AActor* AAuraGameMode::ChoosePlayerStart_Implementation(AController* Player)
{
	UAuraGameInstance* AuraGameInstance = Cast<UAuraGameInstance>(GetGameInstance());
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(),APlayerStart::StaticClass(),FoundActors);
	if (FoundActors.Num() > 0)
	{
		AActor* SelectedActor = FoundActors[0];
		for (AActor* Actor : FoundActors)
		{
			if (APlayerStart* PlayerStart = Cast<APlayerStart>(Actor))
			{
				if (PlayerStart->PlayerStartTag == AuraGameInstance->PlayerStartTag)
				{
					SelectedActor = Actor;
					break;
				}
			}
		}
		return SelectedActor;
	}
	return nullptr;
}


void AAuraGameMode::SaveSlotData(UMVVM_LoadSlot* SaveSlot, int32 SlotIndex)
{
	DeleteSlot(SaveSlot->GetLoadSlotName(),SlotIndex);
	USaveGame* SaveGameObject =	UGameplayStatics::CreateSaveGameObject(LoadScreenSaveGameClass);
	ULoadScreenSaveGame* LoadScreenSaveGame = Cast<ULoadScreenSaveGame>(SaveGameObject);
	LoadScreenSaveGame->PlayerName = SaveSlot->GetPlayerName();
	LoadScreenSaveGame->SlotStatus = SaveSlot->SlotStatus;
	LoadScreenSaveGame->CurMapName = SaveSlot->GetMapName();
	LoadScreenSaveGame->PlayerStartTag = SaveSlot->PlayerStartTag;
	
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

ULoadScreenSaveGame* AAuraGameMode::GetSaveGameSlotByInstance()
{
	UAuraGameInstance* AuraGameInstance = Cast<UAuraGameInstance>(GetGameInstance());
	const FString LoadSlotName = AuraGameInstance->LoadSlotName;
	const int32 SlotIndex = AuraGameInstance->SlotIndex;
	
	return GetSaveGameSlot(LoadSlotName,SlotIndex);
}

void AAuraGameMode::SaveInGameProgressData(ULoadScreenSaveGame* SaveSlot)
{
	UAuraGameInstance* AuraGameInstance = Cast<UAuraGameInstance>(GetGameInstance());
	const FString LoadSlotName = AuraGameInstance->LoadSlotName;
	const int32 SlotIndex = AuraGameInstance->SlotIndex;
	
	AuraGameInstance->PlayerStartTag = SaveSlot->PlayerStartTag;
	UGameplayStatics::SaveGameToSlot(SaveSlot,LoadSlotName,SlotIndex);
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

