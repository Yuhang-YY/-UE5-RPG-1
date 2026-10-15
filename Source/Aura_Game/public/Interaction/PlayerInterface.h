// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PlayerInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPlayerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class AURA_GAME_API IPlayerInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent)
	void AddToXP(int32 XP);
	
	UFUNCTION(BlueprintNativeEvent)
	void AddToPlayerLevel(int32 NewLevel);
	
	UFUNCTION(BlueprintNativeEvent)
	void AddToAttributePoint(int32 NewAttributePoint);
	
	UFUNCTION(BlueprintNativeEvent)
	void AddToSpellPoint(int32 NewSpellPoint);
	
	UFUNCTION(BlueprintNativeEvent) 
	int32 FindLevelForXP(int32 XP);
	
	UFUNCTION(BlueprintNativeEvent)
	void LevelUp();
	
	UFUNCTION(BlueprintNativeEvent) 
	int32 GetXP();
	
	UFUNCTION(BlueprintNativeEvent)
	int32 GetAttributePointsReward(int32 CurLevel)const;
	
	UFUNCTION(BlueprintNativeEvent)
	int32 GetSpellPointsReward(int32 CurLevel)const;
	
	UFUNCTION(BlueprintNativeEvent)
	int32 GetAttributePoint();
	
	UFUNCTION(BlueprintNativeEvent)
	int32 GetSpellPoint();
	
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	void ShowMagicCircleDecal(UMaterialInterface* DecalMaterial = nullptr);
	
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	void HideMagicCircleDecal();
	
	UFUNCTION(BlueprintNativeEvent)
	void SaveProgress(const FName& CheckPointTag);
};
