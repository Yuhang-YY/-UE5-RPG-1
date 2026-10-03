// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AuraWidgetControllerLibrary.generated.h"

struct FGameplayTag;
struct FDamageEffectParams;
class UAuraAbilityInfo;
class USpellMenuWidgetController;
struct FGameplayEffectContextHandle;
class UCharacterClassInfo;
class UAbilitySystemComponent;
enum class ECharacterClass : uint8;
class UAttributeMenuWidgetController;
class UOverlapWidgetController;
/**
 * 
 */
UCLASS()
class AURA_GAME_API UAuraWidgetControllerLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintPure, Category = "Aura")
	static UOverlapWidgetController* GetOverlapWidgetController(UObject* WorldContextObject); 
	
	UFUNCTION(BlueprintPure, Category = "Aura")
	static UAttributeMenuWidgetController* GetAttributeMenuWidgetController(UObject* WorldContextObject);
	
	UFUNCTION(BlueprintPure, Category = "Aura",meta=(DefaultToSelf = "WorldContextObject"))
	static USpellMenuWidgetController* GetSpellMenuWidgetController(UObject* WorldContextObject);
	
	UFUNCTION(BlueprintCallable,Category="Aura")
	static void InitializeCharacterClassInfo(const UObject* WorldContextObject,ECharacterClass CharacterClass,float Level,UAbilitySystemComponent* ASC);
	
	UFUNCTION(BlueprintCallable,Category="Aura")
	static void GiveEnemyStartUpAbilities(const UObject* WorldContextObject,UAbilitySystemComponent* ASC,ECharacterClass CharacterClass);
	
	UFUNCTION(BlueprintCallable,Category="Aura")
	static UCharacterClassInfo* GetCharacterClassInfo(const UObject* WorldContextObject);
	
	UFUNCTION(BlueprintCallable,Category="Aura")
	static UAuraAbilityInfo* GetAbilityInfo(const UObject* WorldContextObject);
	
	UFUNCTION(BlueprintCallable, Category="EffectContext")
	static bool IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle);
	
	UFUNCTION(BlueprintCallable, Category="EffectContext")
	static bool IsSuccessfulDebuff(const FGameplayEffectContextHandle& EffectContextHandle);
	
	UFUNCTION(BlueprintCallable, Category="EffectContext")
	static bool IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle);
	
	UFUNCTION(BlueprintCallable, Category="EffectContext")
	static bool IsRadialDamage(const FGameplayEffectContextHandle& EffectContextHandle);
	
	UFUNCTION(BlueprintCallable, Category="EffectContext")
	static float GetDebuffDuration(const FGameplayEffectContextHandle& EffectContextHandle);
	
	UFUNCTION(BlueprintCallable, Category="EffectContext")
	static float GetDebuffFrequency(const FGameplayEffectContextHandle& EffectContextHandle);
	
	UFUNCTION(BlueprintCallable, Category="EffectContext")
	static float GetDebuffDamage(const FGameplayEffectContextHandle& EffectContextHandle);
	
	UFUNCTION(BlueprintCallable, Category="EffectContext")
	static FGameplayTag GetDamageType(const FGameplayEffectContextHandle& EffectContextHandle);
	
	UFUNCTION(BlueprintCallable, Category="EffectContext")
	static float GetRadialDamageInnerRadius(const FGameplayEffectContextHandle& EffectContextHandle);
	
	UFUNCTION(BlueprintCallable, Category="EffectContext")
	static float GetRadialDamageOuterRadius(const FGameplayEffectContextHandle& EffectContextHandle);
	
	UFUNCTION(BlueprintCallable, Category="EffectContext")
	static FVector GetRadialDamageOrigin(const FGameplayEffectContextHandle& EffectContextHandle);
	
	UFUNCTION(BlueprintCallable, Category="EffectContext")
	static void SetBlockedHit(FGameplayEffectContextHandle& EffectContextHandle,bool bBlockedHit);
	
	UFUNCTION(BlueprintCallable, Category="EffectContext")
	static void SetCriticalHit(FGameplayEffectContextHandle& EffectContextHandle,bool bCriticalHit);
	
	UFUNCTION(BlueprintCallable, Category="EffectContext")
	static void SetSuccessfulDebuff(FGameplayEffectContextHandle& EffectContextHandle,bool bCriticalHit); 
	
	UFUNCTION(BlueprintCallable, Category="EffectContext")
	static void SetDebuffDamage(FGameplayEffectContextHandle& EffectContextHandle,float InDebuffDamage);
	
	UFUNCTION(BlueprintCallable, Category="EffectContext")
	static void SetDebuffDuration(FGameplayEffectContextHandle& EffectContextHandle,float InDebuffDuration);
	
	UFUNCTION(BlueprintCallable, Category="EffectContext")
	static void SetDebuffFrequency(FGameplayEffectContextHandle& EffectContextHandle,float InDebuffFrequency);
	
	UFUNCTION(BlueprintCallable, Category="EffectContext")
	static void SetDamageType(FGameplayEffectContextHandle& EffectContextHandle,const FGameplayTag& InDamageType);
	
	UFUNCTION(BlueprintCallable, Category="EffectContext")
	static void SetIsRadialDamage(FGameplayEffectContextHandle& EffectContextHandle,bool bRadialDamage);
	
	UFUNCTION(BlueprintCallable, Category="EffectContext")
	static void SetRadialDamageInnerRadius(FGameplayEffectContextHandle& EffectContextHandle,float RadialDamageInnerRadius);
	
	UFUNCTION(BlueprintCallable, Category="EffectContext")
	static void SetRadialDamageOuterRadius(FGameplayEffectContextHandle& EffectContextHandle,float RadialDamageOuterRadius);
	
	UFUNCTION(BlueprintCallable, Category="EffectContext")
	static void SetRadialDamageOrigin(FGameplayEffectContextHandle& EffectContextHandle,const FVector& RadialDamageOrigin);
	
	UFUNCTION(BlueprintCallable, Category="EffectContext")
	static void SetDamageEffectParamForTargetASC(UPARAM(ref)FDamageEffectParams& DamageEffectParams, UAbilitySystemComponent* TargetASC);
	
	UFUNCTION(BlueprintCallable, Category="Effect")
	static void GetLifeActorWithingRadius(const UObject* WorldContextObject,TArray<AActor*> &OutActors,const TArray<AActor*> OtherActors,float Radius,FVector SphereLocation);
	
	UFUNCTION(BlueprintCallable, Category="Effect")
	static void GetClosestTargets(int32 MaxTargets,const TArray<AActor*>& Actors,TArray<AActor*>& OutCloseTarget,const FVector& Origin);
	
	UFUNCTION(BlueprintCallable, Category="Effect")
	static bool ISBothFirend(AActor*FirstActor,AActor* SecondActor);
	
	UFUNCTION(BlueprintCallable, Category="Effect")
	static int32 GetXPRewardForClassAndLevel(const UObject* WorldContextObject,ECharacterClass CharacterClass,int32 Level);
	
	UFUNCTION(BlueprintCallable, Category="DamageEffect")
	static FGameplayEffectContextHandle ApplyDamageEffect(const FDamageEffectParams& DamageEffectParams);
	
	UFUNCTION(BlueprintPure)
	static TArray<FRotator> EvenlySpacedRotators(const FVector& Forward,const FVector& Axis,float Spread,int32 NumProjectile);
	
	UFUNCTION(BlueprintPure)
	static TArray<FVector> EvenlyRotatedVectors(const FVector& Forward,const FVector& Axis,float Spread,int32 NumProjectile);
	
	UFUNCTION(BlueprintCallable, Category="DamageEffect")
	static void SetIsRadialDamageEffectParams(UPARAM(ref)FDamageEffectParams& DamageEffectParams,bool bIsRadialDamage,float InnerRadius,float OuterRadius,FVector Origin);
};
