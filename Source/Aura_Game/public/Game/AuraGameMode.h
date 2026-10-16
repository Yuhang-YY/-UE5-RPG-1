// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "AuraGameMode.generated.h"


class ULoadScreenSaveGame;
class USaveGame;
class UMVVM_LoadSlot;
class UAuraAbilityInfo;
class UCharacterClassInfo;
/**
 * 
 */
UCLASS()
class AURA_GAME_API AAuraGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly,Category="CharacterClassInfo")
	TObjectPtr<UCharacterClassInfo> CharacterClassInfo;
	
	UPROPERTY(EditDefaultsOnly,Category="AbilityInfo")
	TObjectPtr<UAuraAbilityInfo> AbilityInfo;
	
	void SaveSlotData(UMVVM_LoadSlot *SaveSlot,int32 SlotIndex);
	
	ULoadScreenSaveGame* GetSaveGameSlot(const FString& SlotName,int32 SlotIndex);
	
	ULoadScreenSaveGame* GetSaveGameSlotByInstance();
	
	void SaveInGameProgressData(ULoadScreenSaveGame* SaveSlot);
	void SaveWorldState(UWorld* World);
	void LoadWorldState(UWorld* World);
	void TravelToMap(UMVVM_LoadSlot* SaveSlot);
	
	
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<USaveGame> LoadScreenSaveGameClass;
	
	//Property
	UPROPERTY(EditDefaultsOnly)
	FName DefaultPlayerStartTag;
	
	UPROPERTY(EditDefaultsOnly)
	FString StartLoadMapName;
	
	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UWorld> DefaultMap;
	
	
	UPROPERTY(EditDefaultsOnly)
	TMap<FString,TSoftObjectPtr<UWorld>> Maps;
	
	static void DeleteSlot(const FString& SlotName,int32 SlotIndex);
protected:
	virtual void BeginPlay() override;
	
	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;
};
