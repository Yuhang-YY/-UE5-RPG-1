// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/AuraGameplayAbility.h"
#include "Interaction/CombatInterface.h"
#include "AuraDamageGameplayAbility.generated.h"

struct FDamageEffectParams;
struct FTaggedMontage;
/**
 * 
 */
UCLASS()
class AURA_GAME_API UAuraDamageGameplayAbility : public UAuraGameplayAbility
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void CauseDamage(AActor* Target);
	
	UFUNCTION(BlueprintCallable)
	FDamageEffectParams MakeDamageEffectParamsFromClassDefaults(AActor* Target = nullptr,FVector RadialDamageOrigin = FVector::ZeroVector)const ;
	
	UFUNCTION(BlueprintPure)
	float GetDamageAtLevel()const;
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile")
	TSubclassOf<UGameplayEffect> DamageEffect;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Damage")
	FGameplayTag DamageType;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Damage")
	FScalableFloat Damage;
	
	/*Debuff*/
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Debuff")
	float DebuffChance = 20.f; 
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Debuff")
	float DebuffDamage = 5.f; 
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Debuff")
	float DebuffFrequency = 1.f; 
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Debuff")
	float DebuffDuration = 5.f; 
	
	/*Debuff*/
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Debuff")
	bool bIsRadialDamage = false; //本次是否启动伤害削减机制
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Debuff")
	float RadialDamageInnerRadius = 0.f; //伤害削减内半径
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Debuff")
	float RadialDamageOuterRadius = 0.f; // 伤害削减外半径
	
	
	
	UFUNCTION(BlueprintCallable)
	FTaggedMontage GetRandomAttackMontageFromArray(const TArray<FTaggedMontage>& MontageArray);
	
};
