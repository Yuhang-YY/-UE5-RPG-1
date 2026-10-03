// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/AuraFireBlast.h"

#include "Actor/AuraFireBall.h"
#include "Library/AuraWidgetControllerLibrary.h"

FString UAuraFireBlast::GetDescriptionCurrent(int32 Level)
{
	return Super::GetDescriptionCurrent(Level);
}

FString UAuraFireBlast::GetNextDescription(int32 Level)
{
	return Super::GetNextDescription(Level); 
}

TArray<AAuraFireBall*> UAuraFireBlast::SpawnFireBalls()
{
	TArray<AAuraFireBall*> ReturnArray;
	
	const FVector& Forward = GetAvatarActorFromActorInfo()->GetActorForwardVector();
	FVector Location = GetAvatarActorFromActorInfo()->GetActorLocation(); 
	TArray<FRotator> SpawnBallVector =  UAuraWidgetControllerLibrary::EvenlySpacedRotators(Forward,FVector::UpVector,360,NumFireBlast);
	
	for (const FRotator& SpawnBall : SpawnBallVector)
	{
		FTransform Transform;
		Transform.SetLocation(Location);
		Transform.SetRotation(SpawnBall.Quaternion());
		
		AAuraFireBall* FireBall = GetWorld()->SpawnActorDeferred<AAuraFireBall>(FireBlastClass,
			Transform,
			GetOwningActorFromActorInfo(),
			CurrentActorInfo->PlayerController->GetPawn(),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		
		FireBall->DamageEffectParams = MakeDamageEffectParamsFromClassDefaults();
		FireBall->ExplosiveParams = MakeDamageEffectParamsFromClassDefaults();
		FireBall->Owner = GetAvatarActorFromActorInfo();
		
		FireBall->ReturnActor = GetAvatarActorFromActorInfo();
		
		ReturnArray.Add(FireBall);
		FireBall->FinishSpawning(Transform);
	}
	
	
	return ReturnArray;
}
