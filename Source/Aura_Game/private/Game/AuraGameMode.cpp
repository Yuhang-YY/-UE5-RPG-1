// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/AuraGameMode.h"

#include "EngineUtils.h"
#include "Aura_Game/AuraLogChannels.h"
#include "Game/AuraGameInstance.h"
#include "Game/LoadScreenSaveGame.h"
#include "GameFramework/PlayerStart.h"
#include "Interaction/SaveInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"
#include "Serialization/MemoryReader.h"
#include "Serialization/MemoryWriter.h"
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

void AAuraGameMode::SaveWorldState(UWorld* World)
{
	if (!IsValid(World)) return;
	FString MapName = World->GetMapName();
	MapName.RemoveFromStart(World->StreamingLevelsPrefix);
	
	UAuraGameInstance* AuraGameInstance = Cast<UAuraGameInstance>(GetGameInstance());
	check(AuraGameInstance);
	
	ULoadScreenSaveGame* SaveGame = GetSaveGameSlot(AuraGameInstance->LoadSlotName,AuraGameInstance->SlotIndex);
	if (!IsValid(SaveGame)) return;
	if (!SaveGame->HasMap(MapName))
	{
		FSavedMap SavedMap;
		SavedMap.MapAssetName = MapName;
		SaveGame->SavedMap.Add(SavedMap);
	}
	
	FSavedMap SavedMap = SaveGame->GetSavedMapWithMapName(MapName);
	SavedMap.SavedActors.Empty();
	
	for (FActorIterator It(World); It ; ++It)
	{
		AActor* Actor = *It;
		if (!IsValid(Actor) || !Actor->Implements<USaveInterface>()) continue;
		
		FSavedActor SavedActor;
		SavedActor.ActorName = Actor->GetFName();
		SavedActor.Transform = Actor->GetTransform();
		
		FMemoryWriter Writer(SavedActor.Bytes);
		FObjectAndNameAsStringProxyArchive Archive(Writer,true);
		Archive.ArIsSaveGame = true; 
		
		Actor->Serialize(Archive);
		
		SavedMap.SavedActors.AddUnique(SavedActor);
	}
	
	for (FSavedMap& MapToReplace: SaveGame->SavedMap)
	{
		if (MapToReplace.MapAssetName == MapName)
		{
			MapToReplace = SavedMap;
		}
	}
	UGameplayStatics::SaveGameToSlot(
	SaveGame,
	AuraGameInstance->LoadSlotName,
	AuraGameInstance->SlotIndex
);
	
}

void AAuraGameMode::LoadWorldState(UWorld* World)
{
	if (!IsValid(World)) return;

	FString MapName = World->GetMapName();
	MapName.RemoveFromStart(World->StreamingLevelsPrefix);

	UAuraGameInstance* AuraGameInstance = Cast<UAuraGameInstance>(GetGameInstance());
	if (!IsValid(AuraGameInstance)) return;

	if (!UGameplayStatics::DoesSaveGameExist(AuraGameInstance->LoadSlotName, AuraGameInstance->SlotIndex)) return;

	ULoadScreenSaveGame* SaveGame = Cast<ULoadScreenSaveGame>(UGameplayStatics::LoadGameFromSlot(
		AuraGameInstance->LoadSlotName, AuraGameInstance->SlotIndex));
	if (!IsValid(SaveGame))
	{
		UE_LOG(LogAura, Error, TEXT("Failed to load slot: %s"), *AuraGameInstance->LoadSlotName);
		return;
	}
	if (!SaveGame->HasMap(MapName)) return;

	const FSavedMap SavedMap = SaveGame->GetSavedMapWithMapName(MapName);
	for (FActorIterator It(World); It; ++It)
	{
		AActor* Actor = *It;
		if (!IsValid(Actor) || !Actor->Implements<USaveInterface>()) continue;

		for (const FSavedActor& SavedActor : SavedMap.SavedActors)
		{
			if (SavedActor.ActorName != Actor->GetFName()) continue;

			if (ISaveInterface::Execute_ShouldLoadTransform(Actor))
			{
				Actor->SetActorTransform(SavedActor.Transform);
			}

			FMemoryReader Reader(SavedActor.Bytes);
			FObjectAndNameAsStringProxyArchive Archive(Reader, true);
			Archive.ArIsSaveGame = true;
			Actor->Serialize(Archive);

			if (Archive.IsError())
			{
				UE_LOG(LogAura, Error, TEXT("Failed to restore actor: %s"), *Actor->GetName());
				break;
			}
			ISaveInterface::Execute_LoadActor(Actor);
			break;
		}
	}
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

