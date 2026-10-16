// Fill out your copyright notice in the Description page of Project Settings.


#include "CheckPoint/CheckPoint.h"
#include "Components/SphereComponent.h"
#include "Game/AuraGameMode.h"
#include "Interaction/PlayerInterface.h"

ACheckPoint::ACheckPoint(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;
	Sphere = CreateDefaultSubobject<USphereComponent>(FName("Sphere"));
	Sphere->SetupAttachment(GetRootComponent());
	Sphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Sphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	Sphere->SetCollisionResponseToChannel(ECC_Pawn,ECR_Overlap);
	
	CheckpointMesh = CreateDefaultSubobject<UStaticMeshComponent>("CheckpointMesh");
	CheckpointMesh->SetupAttachment(GetRootComponent());
	CheckpointMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CheckpointMesh->SetCollisionResponseToAllChannels(ECR_Block);
}

void ACheckPoint::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (HasAuthority() && !bReached && IsValid(OtherActor) && OtherActor->Implements<UPlayerInterface>())
	{
		bReached = true;
		IPlayerInterface::Execute_SaveProgress(OtherActor,PlayerStartTag);
		if (AAuraGameMode* AuraGameMode = GetWorld()->GetAuthGameMode<AAuraGameMode>())
		{
			AuraGameMode->SaveWorldState(GetWorld());
		}
		HandleGlowEffects();
	}
}

bool ACheckPoint::ShouldLoadTransform_Implementation()
{
	return false;
}

void ACheckPoint::LoadActor_Implementation()
{
	if (bReached)
	{
		HandleGlowEffects();
	}
}

void ACheckPoint::BeginPlay()
{
	Super::BeginPlay();
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &ACheckPoint::OnSphereOverlap);
}

void ACheckPoint::HandleGlowEffects()
{
	Sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	// Reuse an existing dynamic material instead of making it another MID's parent.
	if (UMaterialInstanceDynamic* DynamicMaterialInstance = CheckpointMesh->CreateDynamicMaterialInstance(0))
	{
		CheckPointReached(DynamicMaterialInstance);
	}
}
