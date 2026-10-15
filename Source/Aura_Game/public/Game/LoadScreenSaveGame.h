// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/SaveGame.h"
#include "LoadScreenSaveGame.generated.h"

class UGameplayAbility;

UENUM(BlueprintType)
enum ESaveSlotStatus
{
	Vacant,
	EnterName,
	Taken
};

USTRUCT(BlueprintType)
struct FSavedAbility
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ClassDefaults")
	TSubclassOf<UGameplayAbility> GameplayAbility;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FGameplayTag AbilityTag = FGameplayTag();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FGameplayTag AbilityStatus = FGameplayTag();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FGameplayTag AbilitySlot = FGameplayTag();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FGameplayTag AbilityType = FGameplayTag();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 AbilityLevel = 1;
};

/**
 * 
 * 该类是存档游戏对象的基类，可用于保存游戏的状态信息。当你创建自定义的存档子类时，需要为你想要保存的信息添加成员变量。
 * 之后当你想要存档时，使用CreateSaveGameObject创建该对象的实例，填入数据，再调用SaveGameToSlot并提供存档槽名称即可。
 * 要读取存档时，只需调用LoadGameFromSlot，再从返回的对象中读取数据即可。
 */
UCLASS()
class AURA_GAME_API ULoadScreenSaveGame : public USaveGame
{
	GENERATED_BODY()
public:
	UPROPERTY()
	FString SlotName = FString();
	
	UPROPERTY()
	int32 SlotIndex = 0;
	
	UPROPERTY()
	FString PlayerName = FString("Default Name");
	
	UPROPERTY()
	FString CurMapName = FString();
	
	UPROPERTY()
	TEnumAsByte<ESaveSlotStatus> SlotStatus;
	
	UPROPERTY()
	FName PlayerStartTag = FName();
	
	UPROPERTY()
	bool bFirstLoad = true;
	
	
	/*Player*/
	
	UPROPERTY()
	int32 PlayerLevel = 1;
	
	UPROPERTY()
	int32 XP = 0;
	
	UPROPERTY()
	int32 AttributePoint = 0;
	
	UPROPERTY()
	int32 SpellPoint = 0;
	
	UPROPERTY()
	float Strength = 0.0f;
	
	UPROPERTY()
	float Intelligence = 0.0f;
	
	UPROPERTY()
	float Vigor = 0.0f;
	
	UPROPERTY()
	float Resilience = 0.0f;
	
	/*Abilities*/
	UPROPERTY()
	TArray<FSavedAbility> SavedAbilities;

};
