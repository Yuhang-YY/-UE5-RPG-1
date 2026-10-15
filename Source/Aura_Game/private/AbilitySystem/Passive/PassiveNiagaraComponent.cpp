// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Passive/PassiveNiagaraComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "UAuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Interaction/CombatInterface.h"

UPassiveNiagaraComponent::UPassiveNiagaraComponent()
{
	bAutoActivate = false;
}

void UPassiveNiagaraComponent::BeginPlay()
{
	Super::BeginPlay();
	
	if (UAuraAbilitySystemComponent* AuraAsc = Cast<UAuraAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner())))
	{
		AuraAsc->ActivatePassiveEffect.AddUObject(this, &UPassiveNiagaraComponent::OnPassiveActivate);
		const bool bGiveAbility = AuraAsc->bAbilityGiven;
		if (bGiveAbility)
		{
			if (AuraAsc->GetStatusForAbilityTag(PassiveSpellTag) == FUAuraGameplayTags::Get().Ability_Status_Equipped)
			{
				Activate();
			}
		}
	}
	else if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetOwner()))
	{
		CombatInterface->GetOnASCRegisteredDelegate().AddLambda([this](UAbilitySystemComponent* InASC)
		{
			if (UAuraAbilitySystemComponent* AuraAsc = Cast<UAuraAbilitySystemComponent>(InASC))
			{
				AuraAsc->ActivatePassiveEffect.AddUObject(this, &UPassiveNiagaraComponent::OnPassiveActivate);
				const bool bGiveAbility = AuraAsc->bAbilityGiven;
				if (bGiveAbility)
				{
					if (AuraAsc->GetStatusForAbilityTag(PassiveSpellTag) == FUAuraGameplayTags::Get().Ability_Status_Equipped)
					{
						Activate();
					}
				}
			}
		});
	}
}

void UPassiveNiagaraComponent::OnPassiveActivate(const FGameplayTag& AbilityTag, bool bActivate)
{
	if (bActivate)
	{
		if (!IsActive())
		{
			Activate();
		}
	}
	else
	{
		Deactivate();
	}
}
