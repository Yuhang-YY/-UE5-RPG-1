// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "character/AuraCharacterBase.h"
#include "Interaction/PlayerInterface.h"
#include "AuraCharacter.generated.h"

class UNiagaraComponent;
/**
 * 
 */
UCLASS()
class AURA_GAME_API AAuraCharacter : public AAuraCharacterBase,public IPlayerInterface
{
	GENERATED_BODY()
	
public:
	AAuraCharacter();
	virtual int32 GetPlayerLevel_Implementation() override;

	
	//**   playerInterface     **//
	virtual void LevelUp_Implementation() override;
	virtual int32 GetXP_Implementation() override;
	virtual int32 GetAttributePointsReward_Implementation(int32 CurLevel) const override;
	virtual int32 GetSpellPointsReward_Implementation(int32 CurLevel) const override;
	virtual void AddToPlayerLevel_Implementation(int32 NewLevel) override;
	virtual void AddToAttributePoint_Implementation(int32 NewAttributePoint) override;
	virtual void AddToSpellPoint_Implementation(int32 NewSpellPoint) override;
	virtual void AddToXP_Implementation(int32 XP) override;
	virtual int32 FindLevelForXP_Implementation(int32 XP) override;
	virtual int32 GetAttributePoint_Implementation() override;
	virtual int32 GetSpellPoint_Implementation() override;
	virtual void ShowMagicCircleDecal_Implementation(UMaterialInterface* DecalMaterial) override;
	virtual void HideMagicCircleDecal_Implementation() override;
	virtual void SaveProgress_Implementation(const FName& CheckPointTag) override;
	//**   playerInterface     **//
	
	
	//在服务器上，当controller控制Pawn时，调用回调函数，进行组件初始化
	virtual void PossessedBy(AController* NewController) override;
	
	void LoadProgress();
	
	
	//在客户端上，同步数据
	virtual void OnRep_PlayerState() override;
	
	//封装函数用于初始化组件和属性集
	virtual void InitAbilityActorInfo() override;
	
	UFUNCTION(NetMulticast,Reliable)
	void Client_LevelUpNiagaraActive();
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="NiagaraComponent")
	TObjectPtr<UNiagaraComponent> LevelUpNiagaraComponent;
};
