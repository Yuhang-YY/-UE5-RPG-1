// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AuraAbilitySystemComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "UAuraGameplayTags.h"
#include "Abilities/AuraGameplayAbility.h"
#include "AbilitySystem/Data/AuraAbilityInfo.h"
#include "Aura_Game/AuraLogChannels.h"
#include "Game/LoadScreenSaveGame.h"
#include "Interaction/PlayerInterface.h"
#include "Library/AuraWidgetControllerLibrary.h"

void UAuraAbilitySystemComponent::AbilityActorInfoSet()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this,&UAuraAbilitySystemComponent::ClientEffectApplied);
	
	const FUAuraGameplayTags& GameplayTags = FUAuraGameplayTags::Get();
}

void UAuraAbilitySystemComponent::AddCharacterAbilitiesForSaveGame(ULoadScreenSaveGame* SaveGame)
{
	for (const FSavedAbility& SaveData : SaveGame->SavedAbilities)
	{
		const TSubclassOf<UGameplayAbility> AbilityClass = SaveData.GameplayAbility;
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, SaveData.AbilityLevel);
		
		AbilitySpec.DynamicAbilityTags.AddTag(SaveData.AbilitySlot);
		AbilitySpec.DynamicAbilityTags.AddTag(SaveData.AbilityStatus);
		
		if (SaveData.AbilityType == FUAuraGameplayTags::Get().Ability_Type_Offensive)
		{
			GiveAbility(AbilitySpec);
		}
		else if (SaveData.AbilityType == FUAuraGameplayTags::Get().Ability_Type_Passive)
		{
			if (SaveData.AbilityStatus.MatchesTagExact(FUAuraGameplayTags::Get().Ability_Status_Equipped))
			{
				GiveAbilityAndActivateOnce(AbilitySpec);
			}
			else
			{
				GiveAbility(AbilitySpec);
			}
			
		}
	}
	bAbilityGiven = true;
	FAbilityGivenDelegate.Broadcast();
}


void UAuraAbilitySystemComponent::ClientEffectApplied_Implementation(UAbilitySystemComponent* AbilitySystemComponent,
                                                                     const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveGameplayEffectHandle)
{
	FGameplayTagContainer TagContainer;
	EffectSpec.GetAllAssetTags(TagContainer);
	EffectAssetTags.Broadcast(TagContainer);
}

void UAuraAbilitySystemComponent::AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartUpAbilities)
{
	//遍历角色拥有的数组，为每个技能创建登记表(spec)，并添加到ASC中，
	for (const TSubclassOf<UGameplayAbility> AbilityClass  : StartUpAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass,1);
		if (const UAuraGameplayAbility* AuraGameplayAbility = Cast<UAuraGameplayAbility>(AbilitySpec.Ability))
		{
			AbilitySpec.DynamicAbilityTags.AddTag(AuraGameplayAbility->StartupInputTag);
			AbilitySpec.DynamicAbilityTags.AddTag(FUAuraGameplayTags::Get().Ability_Status_Equipped);
			GiveAbility(AbilitySpec);
		}
	}
	bAbilityGiven = true;
	FAbilityGivenDelegate.Broadcast();
	
}

void UAuraAbilitySystemComponent::AddCharacterPassiveAbilities(const TArray<TSubclassOf<UGameplayAbility>>& PassiveAbilities)
{
	for (const TSubclassOf<UGameplayAbility> AbilityClass : PassiveAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass,1);
		AbilitySpec.DynamicAbilityTags.AddTag(FUAuraGameplayTags::Get().Ability_Status_Equipped);
		GiveAbilityAndActivateOnce(AbilitySpec);
	}
}

void UAuraAbilitySystemComponent::AbilityInputPress(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;
	
	//遍历所有可激活的技能
	FScopedAbilityListLock ActiveScopedLock(*this);
	for ( FGameplayAbilitySpec& Spec : GetActivatableAbilities())
	{   //找与输入对应标签的标签
		if (Spec.DynamicAbilityTags.HasTagExact(InputTag))
		{    
			AbilitySpecInputPressed(Spec);
			if (Spec.IsActive())
			{    
				InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputPressed,Spec.Handle,Spec.ActivationInfo.GetActivationPredictionKey());
			} 
		}
	}
}

void UAuraAbilitySystemComponent::AbilityInputTagHeld(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;
	
	//遍历所有可激活的技能
	FScopedAbilityListLock ActiveScopedLock(*this);
	for ( FGameplayAbilitySpec& Spec : GetActivatableAbilities())
	{   //找与输入对应标签的标签
		if (Spec.DynamicAbilityTags.HasTagExact(InputTag))
		{    
			AbilitySpecInputPressed(Spec);
			if (!Spec.IsActive())
			{    //激活
				TryActivateAbility(Spec.Handle);
			}
		}
	}
	
}

void UAuraAbilitySystemComponent::AbilityInputTagRelease(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;
	
	//遍历所有可激活的技能
	FScopedAbilityListLock ActiveScopedLock(*this);
	for ( FGameplayAbilitySpec& Spec : GetActivatableAbilities())
	{   //找与输入对应标签的标签
		if (Spec.DynamicAbilityTags.HasTagExact(InputTag) && Spec.IsActive())
		{     
			AbilitySpecInputReleased(Spec);
			//当技能触发释放保证能触发的关键一步
			InvokeReplicatedEvent( EAbilityGenericReplicatedEvent::InputReleased,Spec.Handle,Spec.ActivationInfo.GetActivationPredictionKey());
		}
	}
}

void UAuraAbilitySystemComponent::ForEachAbility(const FForEachAbility& Delegate)
{
	FScopedAbilityListLock ActiveScopedLock(*this);
	
	for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (!Delegate.ExecuteIfBound(AbilitySpec))
		{
			UE_LOG(LogAura,Error,TEXT("Failed to execute delegate in%hs"),__FUNCTION__);
		}
	}
}

void UAuraAbilitySystemComponent::OnRep_ActivateAbilities()
{
	Super::OnRep_ActivateAbilities();
	
	if (!bAbilityGiven)
	{
		bAbilityGiven = true;
		FAbilityGivenDelegate.Broadcast();
	}
}

FGameplayTag UAuraAbilitySystemComponent::GetAbilityTagFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	if (AbilitySpec.Ability)
	{
		for (const FGameplayTag& AbilityTag : AbilitySpec.Ability.Get()->AbilityTags)
		{
			if (AbilityTag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Ability"))))
			{
				return AbilityTag;
			}
		}
	}
	return FGameplayTag();
}

FGameplayTag UAuraAbilitySystemComponent::GetInputTagFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	for (const FGameplayTag& Tag : AbilitySpec.DynamicAbilityTags)
	{
		if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Input"))))
		{
			return Tag;
		}
	}
	return FGameplayTag();
}

FGameplayTag UAuraAbilitySystemComponent::GetInputTagFromAbilityTag(const FGameplayTag& AbilityTag)
{
	if (FGameplayAbilitySpec* Spec = GetAbilitySpecFromTag(AbilityTag))
	{
		return GetInputTagFromSpec(*Spec);
	}
	return FGameplayTag();
}

FGameplayTag UAuraAbilitySystemComponent::GetStatusTagFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	if (!IsValid(AbilitySpec.Ability)) return FGameplayTag();
	
	for (FGameplayTag StatusTag : AbilitySpec.DynamicAbilityTags)
	{
		if (StatusTag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Ability.Status"))))
		{
			return StatusTag;
		}
	}
	return FGameplayTag();
}

FGameplayTag UAuraAbilitySystemComponent::GetStatusForAbilityTag(const FGameplayTag& AbilityTag)
{
	if (FGameplayAbilitySpec* Spec = GetAbilitySpecFromTag(AbilityTag))
	{
		return GetStatusTagFromSpec(*Spec);
	}
	return FGameplayTag();
}

bool UAuraAbilitySystemComponent::SlotIsEmpty(const FGameplayTag& Slot)
{
	FScopedAbilityListLock ActiveScopedLock(*this);
	for (FGameplayAbilitySpec& Spec : GetActivatableAbilities())
	{
		if (AbilityHasSlot(Spec, Slot))
		{
			return false;
		}
	}
	return true;
}

bool UAuraAbilitySystemComponent::IsPassiveAbility(const FGameplayAbilitySpec& AbilitySpec)
{
	UAuraAbilityInfo* Info = UAuraWidgetControllerLibrary::GetAbilityInfo(GetAvatarActor());
	const FGameplayTag& AbilityTag = GetAbilityTagFromSpec(AbilitySpec);
	const FAbilityInfo AbilityInfo = Info->FindAbilityInfoByTag(AbilityTag);
	return AbilityInfo.AbilityType.MatchesTagExact(FUAuraGameplayTags::Get().Ability_Type_Passive);
}

FGameplayAbilitySpec* UAuraAbilitySystemComponent::GetSpecFromSlot(const FGameplayTag& Slot)
{
	FScopedAbilityListLock ActiveScopedLock(*this);
	for (FGameplayAbilitySpec& Spec : GetActivatableAbilities())
	{
		if (AbilityHasSlot(Spec, Slot))
		{
			return &Spec;
		}
	}
	return nullptr;
}

bool UAuraAbilitySystemComponent::AbilityHasAnySlot(const FGameplayAbilitySpec& Spec)
{
	return Spec.DynamicAbilityTags.HasTag(FGameplayTag::RequestGameplayTag(FName("Input")));
}

void UAuraAbilitySystemComponent::AssignSlotToAbility(FGameplayAbilitySpec& Spec, const FGameplayTag& SlotTag)
{
	ClearSlot(&Spec);
	Spec.DynamicAbilityTags.AddTag(SlotTag);
}

void UAuraAbilitySystemComponent::UpgradeAttribute(const FGameplayTag& AttributeTag)
{
	//如果属性点大于0的时候调用服务器端去更改统一的属性点数据
	if (GetAvatarActor()->Implements<UPlayerInterface>())
	{
		if (IPlayerInterface::Execute_GetAttributePoint(GetAvatarActor()) > 0)
		{
			ServerUpgradeAttribute(AttributeTag);
		}
		
	}
}

void UAuraAbilitySystemComponent::UpdateAbilityStatus(int32 Level)
{
	UAuraAbilityInfo* AbilityInfo = UAuraWidgetControllerLibrary::GetAbilityInfo(GetAvatarActor());
	for (const FAbilityInfo& Info : AbilityInfo->AbilityInformation)
	{
		if (Level < Info.LevelRequirement) continue;
		if (GetAbilitySpecFromTag(Info.AbilityTag) == nullptr)
		{
			FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(Info.GameplayAbility,1);
			AbilitySpec.DynamicAbilityTags.AddTag(FUAuraGameplayTags::Get().Ability_Status_Eligible);
			GiveAbility(AbilitySpec);
			MarkAbilitySpecDirty(AbilitySpec);
			ClientUpdateAbilityStatus(Info.AbilityTag,FUAuraGameplayTags::Get().Ability_Status_Eligible,1);
		}
	}
}

FGameplayAbilitySpec* UAuraAbilitySystemComponent::GetAbilitySpecFromTag(const FGameplayTag& AbilityTag)
{
	FScopedAbilityListLock ActiveScopedLock(*this);
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		for (FGameplayTag Tag : AbilitySpec.Ability.Get()->AbilityTags)
		{
			if (Tag.MatchesTag(AbilityTag))
			{
				return &AbilitySpec;
			}
		}
	}
	return nullptr;
}

void UAuraAbilitySystemComponent::GetAllDescriptionInfo(const FGameplayTag& AbilityTag,FString& CurDescription, FString& NextDescription)
{
	if (FGameplayAbilitySpec* Spec =  GetAbilitySpecFromTag(AbilityTag)) 
	{
		if (UAuraGameplayAbility* Ability = Cast<UAuraGameplayAbility>(Spec->Ability))
		{
			CurDescription = Ability->GetDescriptionCurrent(Spec->Level);
			NextDescription = Ability->GetNextDescription(Spec->Level+1);
		}
	}
	else
	{
		CurDescription = UAuraGameplayAbility::GetLockedDescriptionCurrent();
		NextDescription = FString();
	}
}

void UAuraAbilitySystemComponent::MulticastActivatePassiveEffect_Implementation(const FGameplayTag& AbilityTag,bool IsActivate)
{
	ActivatePassiveEffect.Broadcast(AbilityTag,IsActivate);
}

void UAuraAbilitySystemComponent::ClearSlot(FGameplayAbilitySpec* Spec)
{
	FGameplayTag SlotTag = GetInputTagFromSpec(*Spec);
	Spec->DynamicAbilityTags.RemoveTag(SlotTag);
	MarkAbilitySpecDirty(*Spec);
}

void UAuraAbilitySystemComponent::ClearAbilityOfSlot(FGameplayTag Slot)
{
	FScopedAbilityListLock ActiveScopedLock(*this);
	for (FGameplayAbilitySpec& Spec : GetActivatableAbilities())
	{
		if (AbilityHasSlot(Spec, Slot))
		{
			ClearSlot(&Spec);
		}
	}
}

bool UAuraAbilitySystemComponent::AbilityHasSlot(const FGameplayAbilitySpec& Spec, const FGameplayTag& slot)
{
	return Spec.DynamicAbilityTags.HasTagExact(slot);
}

void UAuraAbilitySystemComponent::ClientEquipAbility_Implementation(const FGameplayTag& AbilityTag,
	const FGameplayTag& Status, const FGameplayTag& Slot, const FGameplayTag& PreSlot)
{
	OnEquipAbility.Broadcast(AbilityTag,Status,Slot,PreSlot);
}

void UAuraAbilitySystemComponent::ServerEquipAbility_Implementation(const FGameplayTag& AbilityTag, const FGameplayTag& Slot)
{
	FUAuraGameplayTags Tags = FUAuraGameplayTags::Get();
	if (FGameplayAbilitySpec* Spec = GetAbilitySpecFromTag(AbilityTag))
	{
		FGameplayTag PreSlot = GetInputTagFromSpec(*Spec);
		FGameplayTag Status = GetStatusTagFromSpec(*Spec);
		
		const bool bStatusValid = Status == FUAuraGameplayTags::Get().Ability_Status_Equipped ||  Status == FUAuraGameplayTags::Get().Ability_Status_Unlocked;
		if (bStatusValid)
		{
			if (!SlotIsEmpty(Slot)) //判断该输入插槽是否被使用，如果使用则清空该插槽
			{
				FGameplayAbilitySpec* SpecWithSlot = GetSpecFromSlot(Slot);
				if (SpecWithSlot)
				{
					//检测这次装备的插槽技能是否是同一个，如果是则直接返回
					if (AbilityTag.MatchesTagExact(GetAbilityTagFromSpec(*SpecWithSlot)))
					{
						ClientEquipAbility(AbilityTag,Tags.Ability_Status_Equipped,Slot,PreSlot);
						return;
					}
					
					//这个技能如果是被动技能则停止
					if (IsPassiveAbility(*SpecWithSlot))
					{
						MulticastActivatePassiveEffect(AbilityTag,false);
						DeactivatePassiveAbility.Broadcast(GetAbilityTagFromSpec(*SpecWithSlot));
					}
					ClearSlot(SpecWithSlot);
				}
			}
			
			if (!AbilityHasAnySlot(*Spec))
			{
				if (IsPassiveAbility(*Spec))
				{
					TryActivateAbility(Spec->Handle);
					MulticastActivatePassiveEffect(AbilityTag,true);
				}
				Spec->DynamicAbilityTags.RemoveTag(GetStatusTagFromSpec(*Spec));
				Spec->DynamicAbilityTags.AddTag(FUAuraGameplayTags::Get().Ability_Status_Equipped);
			}
			AssignSlotToAbility(*Spec,Slot);
			MarkAbilitySpecDirty(*Spec);
			ClientEquipAbility(AbilityTag,Tags.Ability_Status_Equipped,Slot,PreSlot);
		}
	
	}
}

void UAuraAbilitySystemComponent::ServerSpendPointButtonPressed_Implementation(const FGameplayTag& AbilityTag)
{
	
	FUAuraGameplayTags GameplayTags = FUAuraGameplayTags::Get();
	
	if (FGameplayAbilitySpec* AbilitySpec = GetAbilitySpecFromTag(AbilityTag))
	{
		if (GetAvatarActor()->Implements<UPlayerInterface>())
		{
			IPlayerInterface::Execute_AddToSpellPoint(GetAvatarActor(),-1);
		}
		
		FGameplayTag StatusTag = GetStatusTagFromSpec(*AbilitySpec);
		if (StatusTag.MatchesTag(GameplayTags.Ability_Status_Eligible))
		{
			AbilitySpec->DynamicAbilityTags.RemoveTag(GameplayTags.Ability_Status_Eligible);
			AbilitySpec->DynamicAbilityTags.AddTag(GameplayTags.Ability_Status_Unlocked);
			
			StatusTag = GameplayTags.Ability_Status_Unlocked;
		}
		else if (StatusTag.MatchesTag(GameplayTags.Ability_Status_Equipped)||StatusTag.MatchesTag(GameplayTags.Ability_Status_Unlocked) )
		{
			AbilitySpec->Level += 1;
		}
		
		ClientUpdateAbilityStatus(AbilityTag,StatusTag,AbilitySpec->Level);
		MarkAbilitySpecDirty(*AbilitySpec);
	}
}

void UAuraAbilitySystemComponent::ClientUpdateAbilityStatus_Implementation(const FGameplayTag& AbilityTag,const FGameplayTag& StatusTag,const int32& Level)
{
	OnAbilityStatusDelegate.Broadcast(AbilityTag,StatusTag,Level);
	
}

void UAuraAbilitySystemComponent::ServerUpgradeAttribute_Implementation(const FGameplayTag& AttributeTag)
{
	FGameplayEventData Payload;
	Payload.EventTag = AttributeTag;
	Payload.EventMagnitude = 1.f;
	
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetAvatarActor(),AttributeTag,Payload);
	
	if (GetAvatarActor()->Implements<UPlayerInterface>())
	{
		IPlayerInterface::Execute_AddToAttributePoint(GetAvatarActor(),-1);
	} 
		
}


