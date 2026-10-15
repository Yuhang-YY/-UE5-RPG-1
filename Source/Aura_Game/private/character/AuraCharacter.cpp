// Fill out your copyright notice in the Description page of Project Settings.


#include "character/AuraCharacter.h"
#include "AbilitySystemComponent.h"
#include "NiagaraComponent.h"
#include "UAuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AuraAbilityInfo.h"
#include "AbilitySystem/Data/LevelUpInfo.h"
#include "Game/AuraGameMode.h"
#include "Game/LoadScreenSaveGame.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Library/AuraWidgetControllerLibrary.h"
#include "Player/AuraPlayerController.h"
#include "Player/AuraPlayerState.h"
#include "UI/HUD/AuraHUD.h"

AAuraCharacter::AAuraCharacter()
{
	LevelUpNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>("NiagaraComponent");
	LevelUpNiagaraComponent->SetupAttachment(GetRootComponent());
	LevelUpNiagaraComponent->bAutoActivate = false;
	
	
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 400.0f, 0.0f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;
	
	
	
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
	
}

int32 AAuraCharacter::GetPlayerLevel_Implementation()
{
	AAuraPlayerState * AuraPlayerState =  GetPlayerState<AAuraPlayerState>();
	return AuraPlayerState->GetPlayerLevel();
}

void AAuraCharacter::LevelUp_Implementation()
{
	Client_LevelUpNiagaraActive_Implementation();
}

void AAuraCharacter::Client_LevelUpNiagaraActive_Implementation()
{
	if (IsValid(LevelUpNiagaraComponent))
	{
		LevelUpNiagaraComponent->Activate(true);
	}
	
}

int32 AAuraCharacter::GetXP_Implementation()
{
	AAuraPlayerState * AuraPlayerState =  GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	return AuraPlayerState->GetExperience();
}

int32 AAuraCharacter::GetAttributePointsReward_Implementation(int32 CurLevel) const
{
	AAuraPlayerState * AuraPlayerState =  GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	return AuraPlayerState->LeveLInfoPtr->LeveLInformation[CurLevel].AttributePoint;
}

int32 AAuraCharacter::GetSpellPointsReward_Implementation(int32 CurLevel) const
{
	AAuraPlayerState * AuraPlayerState =  GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	return AuraPlayerState->LeveLInfoPtr->LeveLInformation[CurLevel].SpellPoint;
}

void AAuraCharacter::AddToPlayerLevel_Implementation(int32 NewLevel)
{
	AAuraPlayerState * AuraPlayerState =  GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	AuraPlayerState->AddToLevel(NewLevel);
	
	if (UAuraAbilitySystemComponent* ASC = Cast<UAuraAbilitySystemComponent>(GetAbilitySystemComponent()))
	{
		ASC->UpdateAbilityStatus(AuraPlayerState->GetPlayerLevel());
	}
}

void AAuraCharacter::AddToAttributePoint_Implementation(int32 NewAttributePoint)
{
	AAuraPlayerState * AuraPlayerState =  GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	AuraPlayerState->AddToAttributePoints(NewAttributePoint);
}

void AAuraCharacter::AddToSpellPoint_Implementation(int32 NewSpellPoint)
{
	AAuraPlayerState * AuraPlayerState =  GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	AuraPlayerState->AddToSpellPoints(NewSpellPoint);
}

void AAuraCharacter::AddToXP_Implementation(int32 XP)
{
	AAuraPlayerState * AuraPlayerState =  GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	
	AuraPlayerState->AddToXP(XP);
}

int32 AAuraCharacter::FindLevelForXP_Implementation(int32 XP)
{
	AAuraPlayerState * AuraPlayerState =  GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	
	return AuraPlayerState->LeveLInfoPtr->FindLevelForXP(XP);
}

int32 AAuraCharacter::GetAttributePoint_Implementation()
{
	AAuraPlayerState* AuraPlayerState =  GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	return AuraPlayerState->GetAttributePoints();
}

int32 AAuraCharacter::GetSpellPoint_Implementation()
{
	AAuraPlayerState* AuraPlayerState =  GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	return AuraPlayerState->GetSpellPoints();
}

void AAuraCharacter::ShowMagicCircleDecal_Implementation(UMaterialInterface* DecalMaterial)
{
	if (AAuraPlayerController* AuraPlayerController = Cast<AAuraPlayerController>(GetController()))
	{
		AuraPlayerController->ShowMagicCircle(DecalMaterial);
	}
}

void AAuraCharacter::HideMagicCircleDecal_Implementation()
{
	if (AAuraPlayerController* AuraPlayerController = Cast<AAuraPlayerController>(GetController()))
	{
		AuraPlayerController->HideMagicCircle();
	}
}

void AAuraCharacter::SaveProgress_Implementation(const FName& CheckPointTag)
{
	 AAuraGameMode* AuraGameMode = Cast<AAuraGameMode>(UGameplayStatics::GetGameMode(this));
	ULoadScreenSaveGame* LoadScreenSaveGame = AuraGameMode->GetSaveGameSlotByInstance();
	if (LoadScreenSaveGame == nullptr) return;
	
	LoadScreenSaveGame->PlayerStartTag = CheckPointTag;
	if (AAuraPlayerState* AuraPlayerState = Cast<AAuraPlayerState>(GetPlayerState()))
	{
		LoadScreenSaveGame->PlayerLevel = AuraPlayerState->GetPlayerLevel();
		LoadScreenSaveGame->XP = AuraPlayerState->GetExperience();
		LoadScreenSaveGame->AttributePoint = AuraPlayerState->GetAttributePoints();
		LoadScreenSaveGame->SpellPoint = AuraPlayerState->GetSpellPoints();
		
		UAuraAttributeSet* AuraAttributes = Cast<UAuraAttributeSet>(GetAttributeSet());
		LoadScreenSaveGame->Strength = AuraAttributes->GetStrength();
		LoadScreenSaveGame->Intelligence = AuraAttributes->GetIntelligence();
		LoadScreenSaveGame->Resilience = AuraAttributes->GetResilience();
		LoadScreenSaveGame->Vigor = AuraAttributes->GetVigor();
	}
	LoadScreenSaveGame->bFirstLoad = false;
	
	LoadScreenSaveGame->SavedAbilities.Empty();
	if (!HasAuthority())return;
	UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent);
	FForEachAbility SaveAbilityDelegate;
	SaveAbilityDelegate.BindLambda([this, AuraASC, LoadScreenSaveGame](const FGameplayAbilitySpec& Spec)
	{
		const FGameplayTag AbilityTag = AuraASC->GetAbilityTagFromSpec(Spec);
		UAuraAbilityInfo* AbilityInfo = UAuraWidgetControllerLibrary::GetAbilityInfo(this);
		FAbilityInfo Info = AbilityInfo->FindAbilityInfoByTag(AbilityTag);
		
		FSavedAbility SavedAbility;
		SavedAbility.GameplayAbility = Info.GameplayAbility;
		SavedAbility.AbilityLevel = Spec.Level;
		SavedAbility.AbilitySlot = AuraASC->GetInputTagFromSpec(Spec);
		SavedAbility.AbilityStatus = AuraASC->GetStatusTagFromSpec(Spec);
		SavedAbility.AbilityTag = AbilityTag;
		SavedAbility.AbilityType = Info.AbilityType;
		
		LoadScreenSaveGame->SavedAbilities.AddUnique(SavedAbility);
	});
	
	AuraASC->ForEachAbility(SaveAbilityDelegate);
	AuraGameMode->SaveInGameProgressData(LoadScreenSaveGame);
}

void AAuraCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	InitAbilityActorInfo();
	//在服务器端上将技能添加到组件管理
	LoadProgress();
	
}

void AAuraCharacter::LoadProgress()
{
	AAuraGameMode* AuraGameMode = Cast<AAuraGameMode>(UGameplayStatics::GetGameMode(this));
	ULoadScreenSaveGame* LoadScreenSaveGame = AuraGameMode->GetSaveGameSlotByInstance();
	if (LoadScreenSaveGame == nullptr) return;
	
	
	if (LoadScreenSaveGame->bFirstLoad)
	{
		InitializeDefaultAttribute();
		AddCharacterAbilities();
	}
	else
	{
		UAuraAbilitySystemComponent* AuraASC =Cast<UAuraAbilitySystemComponent>(GetAbilitySystemComponent());
		if (AuraASC)
		{
			AuraASC->AddCharacterAbilitiesForSaveGame(LoadScreenSaveGame);
		}
		if (AAuraPlayerState* AuraPlayerState = Cast<AAuraPlayerState>(GetPlayerState()))
		{
			AuraPlayerState->SetLevel(LoadScreenSaveGame->PlayerLevel);
			AuraPlayerState->SetXP(LoadScreenSaveGame->XP);
			AuraPlayerState->SetAttribute(LoadScreenSaveGame->AttributePoint);
			AuraPlayerState->SetSpellPoints(LoadScreenSaveGame->SpellPoint);
		}
		UAuraWidgetControllerLibrary::InitializeAuraDataFromSaveGame(this,GetAbilitySystemComponent(),LoadScreenSaveGame);
	}
}


void AAuraCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	
	InitAbilityActorInfo();
}

void AAuraCharacter::InitAbilityActorInfo()
{
	AAuraPlayerState * AuraPlayerState =  GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	////初始化组件的拥有者和扮演者，拥有者代表逻辑上，扮演者代表世界里的
	AuraPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(AuraPlayerState,this);
	Cast<UAuraAbilitySystemComponent>(AuraPlayerState->GetAbilitySystemComponent())->AbilityActorInfoSet();
	AbilitySystemComponent = AuraPlayerState->GetAbilitySystemComponent();
	AttributeSet = AuraPlayerState->GetAttributeSet();
	OnAscRegistered.Broadcast(AbilitySystemComponent);
	
	if (AAuraPlayerController * AuraPlayerController = Cast<AAuraPlayerController>(GetController()))
	{
		if (AAuraHUD* AuraHUD = Cast<AAuraHUD>(AuraPlayerController->GetHUD()))
		{
			
			AuraHUD->InitOverlap(AuraPlayerController,AuraPlayerState,AbilitySystemComponent,AttributeSet);
		}
	}
	if (IsValid(AbilitySystemComponent))
	{ 
		AbilitySystemComponent->RegisterGameplayTagEvent(FUAuraGameplayTags::Get().Debuff_Stun,EGameplayTagEventType::NewOrRemoved).AddUObject(this,&AAuraCharacter::StunTagChance);
	}
	
}


