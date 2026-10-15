// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ScalableFloat.h"
#include "Engine/DataAsset.h"
#include "CharacterClassInfo.generated.h"

class UGameplayAbility;
class UGameplayEffect;
//RRG角色职业
UENUM(BlueprintType)
enum class ECharacterClass:uint8
{
	Elementalist,
	Warrior,
	Ranger,
};

USTRUCT(BlueprintType)
struct FCharacterClassDefaultInfo
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Default class")
	TSubclassOf<UGameplayEffect> PrimaryAttributes;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Default class")
	TArray<TSubclassOf<UGameplayAbility>> DedicatedAbilities;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Default class")
	FScalableFloat XPReward = FScalableFloat();
};

UCLASS()
class AURA_GAME_API UCharacterClassInfo : public UDataAsset
{
	GENERATED_BODY()
	
public:
	FCharacterClassDefaultInfo GetCharacterClassDefaultInfo(ECharacterClass CharacterClass);
	
	//每个职业对应的主要属性
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category=" Character Class Info ")
	TMap<ECharacterClass, FCharacterClassDefaultInfo> CharacterClassInformation;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Common Class")
	TSubclassOf<UGameplayEffect> PrimaryAttributes_SetByCaller;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Common Class ")
	TSubclassOf<UGameplayEffect> SecondsAttributes;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Common Class ")
	TSubclassOf<UGameplayEffect> SecondsAttributes_Infinite;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Common Class ")
	TSubclassOf<UGameplayEffect> VitalAttributes;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Common Class ")
	TArray<TSubclassOf<UGameplayAbility>> StartUpAbilities;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Damage ")
	TObjectPtr<UCurveTable> DamageCalculationTable;
	
};
