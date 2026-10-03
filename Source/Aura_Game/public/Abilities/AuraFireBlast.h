// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/AuraDamageGameplayAbility.h"
#include "AuraFireBlast.generated.h"

class AAuraFireBall;
/**
 * 
 */
UCLASS()
class AURA_GAME_API UAuraFireBlast : public UAuraDamageGameplayAbility
{
	GENERATED_BODY()
	
public:

	virtual FString GetDescriptionCurrent(int32 Level) override;
	virtual FString GetNextDescription(int32 Level) override;
	
	UFUNCTION(BlueprintCallable)
	TArray<AAuraFireBall*> SpawnFireBalls();
	
protected:
	UPROPERTY(EditDefaultsOnly,Category = "AuraFireBlast")
	int32 NumFireBlast = 12;
	
private:
	UPROPERTY(EditDefaultsOnly,Category = "AuraFireBlast")
	TSubclassOf<AAuraFireBall> FireBlastClass;
};
