// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/AuraPlayerState.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Net/UnrealNetwork.h"

AAuraPlayerState::AAuraPlayerState()
{
	//网络更新频率
	NetUpdateFrequency = 100.0f;
	//初始化Aura的技能系统组件,并将其设置为复制     *多态性*
	AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
	//初始化Aura属性集
	AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>("AttributeSet");
}

void AAuraPlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(AAuraPlayerState,Level);
	DOREPLIFETIME(AAuraPlayerState,Experience);
	DOREPLIFETIME(AAuraPlayerState,AttributePoints);
	DOREPLIFETIME(AAuraPlayerState,SpellPoints);
}

void AAuraPlayerState::OnRep_Level(int32 OldLevel)
{
	OnLevelChangeDelegate.Broadcast(Level);
}

void AAuraPlayerState::OnRep_Experience(int32 Old)
{
	OnXPChangeDelegate.Broadcast(Experience);
}

void AAuraPlayerState::OnRep_AttributePoints(int32 Old)
{
	OnAttributePointChangeDelegate.Broadcast(AttributePoints);
}

void AAuraPlayerState::OnRep_SpellPoints(int32 Old)
{
	OnSpellPointChangeDelegate.Broadcast(SpellPoints);
}

UAbilitySystemComponent* AAuraPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AAuraPlayerState::SetXP(int32 InXp)
{
	Experience = InXp;
	OnXPChangeDelegate.Broadcast(Experience);
}

void AAuraPlayerState::SetLevel(int32 InLevel)
{
	Level = InLevel;
	OnLevelChangeDelegate.Broadcast(Level);
}

void AAuraPlayerState::SetAttribute(int32 InAttributePoints)
{
	AttributePoints = InAttributePoints;
	OnAttributePointChangeDelegate.Broadcast(AttributePoints);
}

void AAuraPlayerState::SetSpellPoints(int32 InSpellPoints)
{
	SpellPoints = InSpellPoints;
	OnSpellPointChangeDelegate.Broadcast(SpellPoints);
}

void AAuraPlayerState::AddToXP(int32 InXP)
{
	Experience+=InXP;
	OnXPChangeDelegate.Broadcast(Experience);
}

void AAuraPlayerState::AddToLevel(int32 InLevel)
{
	Level +=InLevel;
	OnLevelChangeDelegate.Broadcast(Level);
}

void AAuraPlayerState::AddToAttributePoints(int32 InAttributePoints)
{
	AttributePoints += InAttributePoints;
	OnAttributePointChangeDelegate.Broadcast(AttributePoints);
}

void AAuraPlayerState::AddToSpellPoints(int32 InSpellPoints)
{
	SpellPoints += InSpellPoints;
	OnSpellPointChangeDelegate.Broadcast(SpellPoints);
}


