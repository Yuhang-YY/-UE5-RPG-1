// Fill out your copyright notice in the Description page of Project Settings.
/*
 * 此玩家状态，将属性和数据部署在这里是为了防止角色死亡之后数据全部归零，然后保存数据，当角色死亡数据不会丢失，全局只有一个玩家状态
 */
#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "AuraPlayerState.generated.h"

class ULevelUpInfo;
class UAbilitySystemComponent;
class UAttributeSet;


DECLARE_MULTICAST_DELEGATE_OneParam(FGameplayStateChange,int32);
UCLASS()
class AURA_GAME_API AAuraPlayerState : public APlayerState,public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:
	AAuraPlayerState();
	FORCEINLINE int32 GetPlayerLevel()const{return Level;}
	FORCEINLINE int32 GetExperience()const{return Experience;}
	FORCEINLINE int32 GetSpellPoints()const{return SpellPoints;}
	FORCEINLINE int32 GetAttributePoints()const{return AttributePoints;}
	
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	//获取Aura的技能系统组件
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	//获取Aura的属性集
	UAttributeSet* GetAttributeSet() const{return AttributeSet;}
	
	void SetXP(int32 InXp);
	void SetLevel(int32 InLevel);
	void SetAttribute(int32 InAttributePoints);
	void SetSpellPoints(int32 InSpellPoints);
	
	void AddToXP(int32 InXP);
	void AddToLevel(int32 InLevel);
	void AddToAttributePoints(int32 InAttributePoints);
	void AddToSpellPoints(int32 InSpellPoints);
	
	FGameplayStateChange OnXPChangeDelegate;
	FGameplayStateChange OnLevelChangeDelegate;
	FGameplayStateChange OnAttributePointChangeDelegate;
	FGameplayStateChange OnSpellPointChangeDelegate;
	
	//等级信息资产指针
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<ULevelUpInfo> LeveLInfoPtr;
protected:
	//技能系统组件
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	
	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;
	
private:
	UPROPERTY(VisibleAnywhere,ReplicatedUsing=OnRep_Level)
	int32 Level = 1;
	
	UPROPERTY(VisibleAnywhere,ReplicatedUsing=OnRep_Experience)
	int32 Experience = 0;
	
	UPROPERTY(VisibleAnywhere,ReplicatedUsing=OnRep_AttributePoints)
	int32 AttributePoints = 0;
	
	UPROPERTY(VisibleAnywhere,ReplicatedUsing=OnRep_SpellPoints)
	int32 SpellPoints = 1;
	
	
	UFUNCTION()
	void OnRep_Level(int32 OldLevel);
	
	UFUNCTION()
	void OnRep_Experience(int32 Old);
	
	UFUNCTION()
	void OnRep_AttributePoints(int32 Old);
	
	UFUNCTION()
	void OnRep_SpellPoints(int32 Old);
	
	
	
};
