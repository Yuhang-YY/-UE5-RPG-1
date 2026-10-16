// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerStart.h"
#include "Interaction/SaveInterface.h"
#include "CheckPoint.generated.h"

class USphereComponent;
class UStaticMeshComponent;

/**
 * 
 */
UCLASS()
class AURA_GAME_API ACheckPoint : public APlayerStart,public ISaveInterface
{
	GENERATED_BODY()
	
public:
	ACheckPoint(const FObjectInitializer& ObjectInitializer);

	virtual bool ShouldLoadTransform_Implementation() override;
	virtual void LoadActor_Implementation() override;

	UPROPERTY(BlueprintReadOnly,SaveGame)
	bool bReached = false;
protected:
	UFUNCTION()
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	virtual void BeginPlay()override;
	
	UFUNCTION(BlueprintImplementableEvent)
	void CheckPointReached(UMaterialInstanceDynamic* DynamicMaterialInstance);
	
	void HandleGlowEffects();
private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> CheckpointMesh;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> Sphere;
};
