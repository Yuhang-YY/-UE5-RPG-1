// Fill out your copyright notice in the Description page of Project Settings.


#include "Library/AuraWidgetControllerLibrary.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AuraAbilityType.h"
#include "UAuraGameplayTags.h"
#include "Game/AuraGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Player/AuraPlayerState.h"
#include "UI/HUD/AuraHUD.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Game/LoadScreenSaveGame.h"
#include "Interaction/CombatInterface.h"
#include "UI/Widget/AuraWidgetController.h"
#include "Widgets/SDMXReadOnlyFixturePatchList.h"

UOverlapWidgetController* UAuraWidgetControllerLibrary::GetOverlapWidgetController(UObject* WorldContextObject)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject,0))
	{
		if (AAuraHUD* HUD = Cast<AAuraHUD>(PC->GetHUD()))
		{
			AAuraPlayerState* PS = PC->GetPlayerState<AAuraPlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();
			const FWidgetControllerParams Params(PC,PS,ASC,AS);
			return HUD->GetOverlapWidgetController(Params);
		}
	}
	return nullptr;
	
}

UAttributeMenuWidgetController* UAuraWidgetControllerLibrary::GetAttributeMenuWidgetController(UObject* WorldContextObject)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject,0))
	{
		if (AAuraHUD* HUD = Cast<AAuraHUD>(PC->GetHUD()))
		{
			
			AAuraPlayerState* PS = PC->GetPlayerState<AAuraPlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			 UAttributeSet* AS = PS->GetAttributeSet();
			const FWidgetControllerParams Params(PC,PS,ASC,AS);
			return HUD->GetAttributeMenuController(Params);
		}
	}
	return nullptr;
}

USpellMenuWidgetController* UAuraWidgetControllerLibrary::GetSpellMenuWidgetController(UObject* WorldContextObject)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject,0))
	{
		if (AAuraHUD* HUD = Cast<AAuraHUD>(PC->GetHUD()))
		{
			AAuraPlayerState* PS = PC->GetPlayerState<AAuraPlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();
			const FWidgetControllerParams Params(PC,PS,ASC,AS);
			return HUD->GetSpellMenuController(Params);
		}
	}
	return nullptr;
}

void UAuraWidgetControllerLibrary::InitializeCharacterClassInfo(const UObject* WorldContextObject,ECharacterClass CharacterClass, float Level,UAbilitySystemComponent* ASC)
{
	AAuraGameMode* Mode =Cast<AAuraGameMode>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (Mode==nullptr) return;
	
	//从mode获取资产，然后调用根据职业获取对应的效果
	UCharacterClassInfo* CharacterClassInfo  = Mode->CharacterClassInfo;
	const FCharacterClassDefaultInfo CharacterClassDefaultInfo = CharacterClassInfo->GetCharacterClassDefaultInfo(CharacterClass);
	
	FGameplayEffectContextHandle EffectContextHandle = ASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(ASC->GetAvatarActor());
	const FGameplayEffectSpecHandle EffectSpecHandle = ASC->MakeOutgoingSpec(CharacterClassDefaultInfo.PrimaryAttributes,Level,EffectContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
	
	
	FGameplayEffectContextHandle SecondEffectContextHandle = ASC->MakeEffectContext();
	SecondEffectContextHandle.AddSourceObject(ASC->GetAvatarActor());
	const FGameplayEffectSpecHandle SecondEffectSpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->SecondsAttributes,Level,SecondEffectContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*SecondEffectSpecHandle.Data.Get());
	
	
	FGameplayEffectContextHandle VitalEffectContextHandle = ASC->MakeEffectContext();
	VitalEffectContextHandle.AddSourceObject(ASC->GetAvatarActor());
	const FGameplayEffectSpecHandle VitalEffectSpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->VitalAttributes,Level,VitalEffectContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*VitalEffectSpecHandle.Data.Get());
	
}

void UAuraWidgetControllerLibrary::InitializeAuraDataFromSaveGame(const UObject* WorldContextObject,UAbilitySystemComponent* ASC, ULoadScreenSaveGame* SaveGame)
{
	AAuraGameMode* Mode =Cast<AAuraGameMode>(UGameplayStatics::GetGameMode(WorldContextObject));
	UCharacterClassInfo* CharacterClassInfo  = Mode->CharacterClassInfo;
	if (Mode==nullptr && CharacterClassInfo) return;
	
	AActor* SourActor = ASC->GetAvatarActor();
	
	FGameplayEffectContextHandle EffectContextHandle = ASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(ASC->GetAvatarActor());
	FGameplayEffectSpecHandle EffectSpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->PrimaryAttributes_SetByCaller,1.f,EffectContextHandle);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(EffectSpecHandle,FUAuraGameplayTags::Get().Attribute_Primary_Strength,SaveGame->Strength);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(EffectSpecHandle,FUAuraGameplayTags::Get().Attribute_Primary_Intelligence,SaveGame->Intelligence);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(EffectSpecHandle,FUAuraGameplayTags::Get().Attribute_Primary_Resilience,SaveGame->Resilience);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(EffectSpecHandle,FUAuraGameplayTags::Get().Attribute_Primary_Vigor,SaveGame->Vigor);
	ASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
	
	FGameplayEffectContextHandle SecondEffectContextHandle = ASC->MakeEffectContext();
	SecondEffectContextHandle.AddSourceObject(ASC->GetAvatarActor());
	const FGameplayEffectSpecHandle SecondEffectSpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->SecondsAttributes_Infinite,1.f,SecondEffectContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*SecondEffectSpecHandle.Data.Get());
	
	
	FGameplayEffectContextHandle VitalEffectContextHandle = ASC->MakeEffectContext();
	VitalEffectContextHandle.AddSourceObject(ASC->GetAvatarActor());
	const FGameplayEffectSpecHandle VitalEffectSpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->VitalAttributes,1.f,VitalEffectContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*VitalEffectSpecHandle.Data.Get());
	
}

void UAuraWidgetControllerLibrary::GiveEnemyStartUpAbilities(const UObject* WorldContextObject,UAbilitySystemComponent* ASC,ECharacterClass CharacterClass)
{
	AAuraGameMode* Mode =Cast<AAuraGameMode>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (Mode==nullptr) return;
	
	for (const TSubclassOf<UGameplayAbility>& CharacterClassInfo : Mode->CharacterClassInfo->StartUpAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(CharacterClassInfo,1);
		ASC->GiveAbility(AbilitySpec);
	}
	
	FCharacterClassDefaultInfo DefaultInfo = Mode->CharacterClassInfo->GetCharacterClassDefaultInfo(CharacterClass);
	if (ASC->GetAvatarActor()->Implements<UCombatInterface>())
	{
		for (TSubclassOf<UGameplayAbility> Ability : DefaultInfo.DedicatedAbilities)
		{
		
			FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(Ability,ICombatInterface::Execute_GetPlayerLevel(ASC->GetAvatarActor()));
			ASC->GiveAbility(Ability);
		}
	}
	
}

UCharacterClassInfo* UAuraWidgetControllerLibrary::GetCharacterClassInfo(const UObject* WorldContextObject)
{
	const AAuraGameMode* Mode =Cast<AAuraGameMode>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (Mode==nullptr) return nullptr;
	
	return  Mode->CharacterClassInfo;
}

UAuraAbilityInfo* UAuraWidgetControllerLibrary::GetAbilityInfo(const UObject* WorldContextObject)
{
	const AAuraGameMode* Mode =Cast<AAuraGameMode>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (Mode==nullptr) return nullptr;
	
	return  Mode->AbilityInfo;
}

bool UAuraWidgetControllerLibrary::IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FAuraGameplayEffectContext* AuraContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraContext->IsBlockHit();
	}
	return false;
}

bool UAuraWidgetControllerLibrary::IsSuccessfulDebuff(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FAuraGameplayEffectContext* AuraContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraContext->IsSuccessfulDebuff();
	}
	return false;
}

float UAuraWidgetControllerLibrary::GetDebuffDuration(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FAuraGameplayEffectContext* AuraContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraContext->GetDebuffDuration();
	}
	return 0.f;
}

float UAuraWidgetControllerLibrary::GetDebuffFrequency(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FAuraGameplayEffectContext* AuraContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraContext->GetDebuffFrequency();
	}
	return 0.f;
}

float UAuraWidgetControllerLibrary::GetDebuffDamage(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FAuraGameplayEffectContext* AuraContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraContext->GetDebuffDamage();
	}
	return 0.f;
}

FGameplayTag UAuraWidgetControllerLibrary::GetDamageType(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FAuraGameplayEffectContext* AuraContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return *AuraContext->GetDamageType();
	}
	return FGameplayTag();
}

float UAuraWidgetControllerLibrary::GetRadialDamageInnerRadius(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FAuraGameplayEffectContext* AuraContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraContext->GetRadialDamageInnerRadius();
	}
	return 0.f;
}

float UAuraWidgetControllerLibrary::GetRadialDamageOuterRadius(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FAuraGameplayEffectContext* AuraContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraContext->GetRadialDamageOuterRadius();
	}
	return 0.f;
}

FVector UAuraWidgetControllerLibrary::GetRadialDamageOrigin(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FAuraGameplayEffectContext* AuraContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraContext->GetRadialDamageOrigin();
	}
	return FVector();
}

bool UAuraWidgetControllerLibrary::IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FAuraGameplayEffectContext* AuraContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraContext->IsCriticalHit();
	}
	return false;
}

bool UAuraWidgetControllerLibrary::IsRadialDamage(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FAuraGameplayEffectContext* AuraContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraContext->IsRadialDamage();
	}
	return false;
}

void UAuraWidgetControllerLibrary::SetBlockedHit(FGameplayEffectContextHandle& EffectContextHandle, bool bBlockedHit)
{
	if ( FAuraGameplayEffectContext* AuraContext = static_cast< FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		AuraContext->SetBlockHit(bBlockedHit);
	}
}

void UAuraWidgetControllerLibrary::SetCriticalHit(FGameplayEffectContextHandle& EffectContextHandle, bool bCriticalHit)
{
	if ( FAuraGameplayEffectContext* AuraContext = static_cast< FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		AuraContext->SetCriticalHit(bCriticalHit);
	}
}

void UAuraWidgetControllerLibrary::SetSuccessfulDebuff(FGameplayEffectContextHandle& EffectContextHandle,bool bCriticalHit)
{
	if ( FAuraGameplayEffectContext* AuraContext = static_cast< FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		AuraContext->SetSuccessfulDebuff(bCriticalHit);
	}
}

void UAuraWidgetControllerLibrary::SetDebuffDamage(FGameplayEffectContextHandle& EffectContextHandle,float InDebuffDamage)
{
	if ( FAuraGameplayEffectContext* AuraContext = static_cast< FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		AuraContext->SetDebuffDamage(InDebuffDamage);
	}
}

void UAuraWidgetControllerLibrary::SetDebuffDuration(FGameplayEffectContextHandle& EffectContextHandle,float InDebuffDuration)
{
	if ( FAuraGameplayEffectContext* AuraContext = static_cast< FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		AuraContext->SetDebuffDuration(InDebuffDuration);
	}
}

void UAuraWidgetControllerLibrary::SetDebuffFrequency(FGameplayEffectContextHandle& EffectContextHandle,float InDebuffFrequency)
{
	if ( FAuraGameplayEffectContext* AuraContext = static_cast< FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		AuraContext->SetDebuffFrequency(InDebuffFrequency);
	}
}

void UAuraWidgetControllerLibrary::SetDamageType(FGameplayEffectContextHandle& EffectContextHandle,const FGameplayTag& InDamageType)
{
	if ( FAuraGameplayEffectContext* AuraContext = static_cast< FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		TSharedPtr<FGameplayTag> DamageType = MakeShared<FGameplayTag>(InDamageType);
		AuraContext->SetDamageType(DamageType);
	}
}

void UAuraWidgetControllerLibrary::SetIsRadialDamage(FGameplayEffectContextHandle& EffectContextHandle,bool bRadialDamage)
{
	if ( FAuraGameplayEffectContext* AuraContext = static_cast< FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		AuraContext->SetRadialDamage(bRadialDamage);
	}
}

void UAuraWidgetControllerLibrary::SetRadialDamageInnerRadius(FGameplayEffectContextHandle& EffectContextHandle,float RadialDamageInnerRadius)
{
	if ( FAuraGameplayEffectContext* AuraContext = static_cast< FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		AuraContext->SetRadialDamageInnerRadius(RadialDamageInnerRadius);
	}
}

void UAuraWidgetControllerLibrary::SetRadialDamageOuterRadius(FGameplayEffectContextHandle& EffectContextHandle,float RadialDamageOuterRadius)
{
	if ( FAuraGameplayEffectContext* AuraContext = static_cast< FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		AuraContext->SetRadialDamageOuterRadius(RadialDamageOuterRadius);
	}
}

void UAuraWidgetControllerLibrary::SetRadialDamageOrigin(FGameplayEffectContextHandle& EffectContextHandle,const FVector& RadialDamageOrigin)
{
	if ( FAuraGameplayEffectContext* AuraContext = static_cast< FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		AuraContext->SetRadialDamageOrigin(RadialDamageOrigin);
	}
}

void UAuraWidgetControllerLibrary::SetDamageEffectParamForTargetASC(FDamageEffectParams& DamageEffectParams, UAbilitySystemComponent* TargetASC)
{
	DamageEffectParams.TargetAbilitySystemComponent = TargetASC;
}

void UAuraWidgetControllerLibrary::GetLifeActorWithingRadius(const UObject* WorldContextObject,TArray<AActor*>& OutActors, const TArray<AActor*> OtherActors, float Radius, FVector SphereLocation)
{
	FCollisionQueryParams SphereParams;
	SphereParams.AddIgnoredActors(OtherActors);

	if (const UWorld*World = GEngine->GetWorldFromContextObject(WorldContextObject,EGetWorldErrorMode::LogAndReturnNull))
	{
		TArray<FOverlapResult> Overlaps;
		World->OverlapMultiByObjectType(Overlaps, SphereLocation, FQuat::Identity, FCollisionObjectQueryParams(FCollisionObjectQueryParams::InitType::AllDynamicObjects), FCollisionShape::MakeSphere(Radius), SphereParams);
		
		for (FOverlapResult &Overlap : Overlaps)
		{
			bool ImplementCombatInterface = Overlap.GetActor()->Implements<UCombatInterface>();
			if (ImplementCombatInterface )
			{
				bool IsAlive = !ICombatInterface::Execute_IsDead(Overlap.GetActor());
			
				if (IsAlive)
				{
					OutActors.AddUnique(Overlap.GetActor());
				}
			}
			
		}
	}
}

void UAuraWidgetControllerLibrary::GetClosestTargets(int32 MaxTargets, const TArray<AActor*>& Actors,TArray<AActor*>& OutCloseTargets,const FVector& Origin)
{
	
	
	if (Actors.Num() <= MaxTargets)
	{
		OutCloseTargets = Actors;
		return;
	}
	
	TArray<AActor*> ActorsToCheck;
	int32 NumTargetsFound = 0;
	
	while (NumTargetsFound < MaxTargets)
	{
		if (ActorsToCheck.Num() == 0) break;
		double ClosestDistance = TNumericLimits<double>::Max();
		AActor* ClosestActor;
		for (AActor* Actor : ActorsToCheck)
		{
			const double LocalDistance = (Actor->GetActorLocation() - Origin).Length();
			if (LocalDistance < ClosestDistance)
			{
				ClosestDistance = LocalDistance;
				ClosestActor = Actor;
			}
		}
		ActorsToCheck.Remove(ClosestActor);
		OutCloseTargets.Add(ClosestActor);
		NumTargetsFound++;
	}
}

bool UAuraWidgetControllerLibrary::ISBothFirend(AActor* FirstActor, AActor* SecondActor)
{
	const bool ISBothPlayer = FirstActor->ActorHasTag(FName("Player"))&&SecondActor->ActorHasTag(FName("Player"));
	const bool IsBothEnemy = FirstActor->ActorHasTag(FName("Enemy"))&&SecondActor->ActorHasTag(FName("Enemy"));
	
	const bool IsFriend = IsBothEnemy || ISBothPlayer;
	
	return !IsFriend;
}

int32 UAuraWidgetControllerLibrary::GetXPRewardForClassAndLevel(const UObject* WorldContextObject,ECharacterClass CharacterClass, int32 Level)
{
	 UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	if (CharacterClassInfo==nullptr) return 0.f;
	
	const FCharacterClassDefaultInfo& ClassInfo = CharacterClassInfo->GetCharacterClassDefaultInfo(CharacterClass);
	float XP = ClassInfo.XPReward.GetValueAtLevel(Level);
	
	return static_cast<int32>(XP);
}

FGameplayEffectContextHandle UAuraWidgetControllerLibrary::ApplyDamageEffect(const FDamageEffectParams& DamageEffectParams)
{
	if (!IsValid(DamageEffectParams.SourceAbilitySystemComponent.Get()))
	{
		UE_LOG(LogTemp, Warning, TEXT("ApplyDamageEffect skipped: SourceAbilitySystemComponent is invalid."));
		return FGameplayEffectContextHandle();
	}
	if (!IsValid(DamageEffectParams.TargetAbilitySystemComponent.Get()))
	{
		UE_LOG(LogTemp, Warning, TEXT("ApplyDamageEffect skipped: TargetAbilitySystemComponent is invalid. Set the target ASC on the damage parameters before applying damage."));
		return FGameplayEffectContextHandle();
	}
	if (!DamageEffectParams.DamageGameplayEffectClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("ApplyDamageEffect skipped: DamageGameplayEffectClass is not set."));
		return FGameplayEffectContextHandle();
	}

	FUAuraGameplayTags Tags = FUAuraGameplayTags::Get();
	
	FGameplayEffectContextHandle EffectContextHandle = DamageEffectParams.SourceAbilitySystemComponent->MakeEffectContext();
	EffectContextHandle.AddSourceObject(DamageEffectParams.SourceAbilitySystemComponent->GetAvatarActor());
	SetIsRadialDamage(EffectContextHandle,DamageEffectParams.bIsRadialDamage);
	SetRadialDamageInnerRadius(EffectContextHandle,DamageEffectParams.RadialDamageInnerRadius);
	SetRadialDamageOuterRadius(EffectContextHandle,DamageEffectParams.RadialDamageOuterRadius);
	SetRadialDamageOrigin(EffectContextHandle,DamageEffectParams.RadialDamageOrigin);
	
	FGameplayEffectSpecHandle SpecHandle = DamageEffectParams.SourceAbilitySystemComponent->MakeOutgoingSpec(DamageEffectParams.DamageGameplayEffectClass,DamageEffectParams.AbilityLevel,EffectContextHandle);
	if (!SpecHandle.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("ApplyDamageEffect skipped: MakeOutgoingSpec returned an invalid spec."));
		return FGameplayEffectContextHandle();
	}
	
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle,DamageEffectParams.DamageType,DamageEffectParams.BaseDamage);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle,Tags.Debuff_Chance,DamageEffectParams.DebuffChance);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle,Tags.Debuff_Damage,DamageEffectParams.DebuffDamage);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle,Tags.Debuff_Duration,DamageEffectParams.DebuffDamageDuration);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle,Tags.Debuff_Frequency,DamageEffectParams.DebuffFrequency);
	
	DamageEffectParams.TargetAbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data);
	
	return EffectContextHandle;
	
}

TArray<FRotator> UAuraWidgetControllerLibrary::EvenlySpacedRotators(const FVector& Forward, const FVector& Axis,float Spread,int32 NumProjectile)
{
	TArray<FRotator> Rotators;
	
	FVector LeftOfSpread = Forward.RotateAngleAxis(-Spread / 2.f,Axis);
	if (NumProjectile > 1)
	{
		const float DeltaSpread = Spread / (NumProjectile - 1);
		for ( int32 i = 0; i < NumProjectile; ++i)
		{
			const FVector Direction = LeftOfSpread.RotateAngleAxis(DeltaSpread * i,FVector::UpVector);
			Rotators.Add(Direction.Rotation());
		}
	}
	else
	{
		Rotators.Add(Forward.Rotation());	
	}
	
	return Rotators;
}

TArray<FVector> UAuraWidgetControllerLibrary::EvenlyRotatedVectors(const FVector& Forward, const FVector& Axis,float Spread,int32 NumProjectile)
{
	TArray<FVector> Vectors;
	FVector LeftOfSpread = Forward.RotateAngleAxis(-Spread / 2.f,Axis);
	if (NumProjectile > 1)
	{
		const float DeltaSpread = Spread / (NumProjectile - 1);
		for ( int32 i = 0; i < NumProjectile; ++i)
		{
			const FVector Direction = LeftOfSpread.RotateAngleAxis(DeltaSpread * i,FVector::UpVector);
			Vectors.Add(Direction);
		}
	}
	else
	{
		Vectors.Add(Forward);	
	}
	return Vectors;
}

void UAuraWidgetControllerLibrary::SetIsRadialDamageEffectParams(FDamageEffectParams& DamageEffectParams,bool bIsRadialDamage, float InnerRadius, float OuterRadius, FVector Origin)
{
	DamageEffectParams.bIsRadialDamage = bIsRadialDamage;
	DamageEffectParams.RadialDamageInnerRadius = InnerRadius;
	DamageEffectParams.RadialDamageOuterRadius = OuterRadius;
	DamageEffectParams.RadialDamageOrigin = Origin;
}
