// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AuraAbilitySystemComponent.generated.h"

class ULoadScreenSaveGame;
class UAuraAbilitySystemComponent;
struct FGameplayTag;


DECLARE_MULTICAST_DELEGATE_OneParam(FEffectAssetTags, const FGameplayTagContainer& AssetTags);
DECLARE_MULTICAST_DELEGATE(FAbilitiesGiven);
DECLARE_DELEGATE_OneParam(FForEachAbility,const FGameplayAbilitySpec&);
DECLARE_MULTICAST_DELEGATE_ThreeParams(FAbilityStatusDelegate, const FGameplayTag& /** AbilityTag**/, const FGameplayTag&/** StatusTag**/,int32);
DECLARE_MULTICAST_DELEGATE_FourParams(FEquipAbilityDelegate, const FGameplayTag& /** AbilityTag**/, const FGameplayTag&/** StatusTag**/,const FGameplayTag&/** slot**/,const FGameplayTag&/** preSlot**/);
DECLARE_MULTICAST_DELEGATE_OneParam(FDeactivatePassiveAbility, const FGameplayTag& /*AbilityTag*/); //当停用被动技能时调用的委托
DECLARE_MULTICAST_DELEGATE_TwoParams(FActivatePassiveEffect,const FGameplayTag& ,bool );
/**
 * 
 */
UCLASS()
class AURA_GAME_API UAuraAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
	
public:
	//初始化effect应用时调用委托把数据传过去
	void AbilityActorInfoSet();
	
	void AddCharacterAbilitiesForSaveGame(ULoadScreenSaveGame* SaveGame);
	void AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartUpAbilities);
	void AddCharacterPassiveAbilities(const TArray<TSubclassOf<UGameplayAbility>>& PassiveAbilities);
	
	void AbilityInputPress(const FGameplayTag& InputTag);
	void AbilityInputTagHeld(const FGameplayTag& InputTag);
	void AbilityInputTagRelease(const FGameplayTag& InputTag);
	void ForEachAbility(const FForEachAbility& Delegate);
	
	virtual  void OnRep_ActivateAbilities()override;
	
	
	static FGameplayTag GetAbilityTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);
	static FGameplayTag GetInputTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);
	FGameplayTag GetInputTagFromAbilityTag(const FGameplayTag& AbilityTag);
	static FGameplayTag GetStatusTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);
	FGameplayTag GetStatusForAbilityTag(const FGameplayTag& AbilityTag);
	bool SlotIsEmpty(const FGameplayTag& Slot);
	bool IsPassiveAbility(const FGameplayAbilitySpec& AbilitySpec);
	FGameplayAbilitySpec* GetSpecFromSlot(const FGameplayTag& Slot);
	bool AbilityHasAnySlot(const FGameplayAbilitySpec& Spec);
	void AssignSlotToAbility(FGameplayAbilitySpec& Spec,const FGameplayTag& SlotTag);
	
	
	void UpgradeAttribute(const FGameplayTag& AttributeTag);
	void UpdateAbilityStatus(int32 Level);
	
	FGameplayAbilitySpec* GetAbilitySpecFromTag(const FGameplayTag&AbilityTag);
	
	void GetAllDescriptionInfo(const FGameplayTag& AbilityTag,FString &CurDescription,FString& NextDescription);
	
	UFUNCTION(Server, Reliable)
	void ServerUpgradeAttribute(const FGameplayTag& AttributeTag);
	
	UFUNCTION(Server, Reliable)
	void ServerSpendPointButtonPressed(const FGameplayTag& AbilityTag);
	
	UFUNCTION(Server, Reliable)
	void ServerEquipAbility(const FGameplayTag& AbilityTag,const FGameplayTag& Slot);
	
	UFUNCTION(NetMulticast,Unreliable)
	void MulticastActivatePassiveEffect(const FGameplayTag& AbilityTag,bool IsActivate);
	
	void ClearSlot(FGameplayAbilitySpec* Spec);
	void ClearAbilityOfSlot(FGameplayTag Slot);
	bool AbilityHasSlot(const FGameplayAbilitySpec& Spec,const FGameplayTag& slot);
	
	FEffectAssetTags EffectAssetTags;
	FAbilitiesGiven FAbilityGivenDelegate;
	FAbilityStatusDelegate OnAbilityStatusDelegate;
	FEquipAbilityDelegate OnEquipAbility;
	FDeactivatePassiveAbility DeactivatePassiveAbility; //停用被动技能委托实例
	FActivatePassiveEffect ActivatePassiveEffect;
	
	bool bAbilityGiven = false;
protected:
	UFUNCTION(Client,Reliable)
	void ClientEffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveGameplayEffectHandle);

	UFUNCTION(Client,Reliable)
	void ClientUpdateAbilityStatus(const FGameplayTag& AbilityTag,const FGameplayTag& StatusTag,const int32& Level);
	
	UFUNCTION(Client,Reliable)
	void ClientEquipAbility(const FGameplayTag& AbilityTag,const FGameplayTag& Status,const FGameplayTag& Slot,const FGameplayTag& PreSlot);
};
