


#include "Actor/AuraProjectile.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "Aura_Game/Aura_Game.h"
#include "Components/AudioComponent.h"
#include "Library/AuraWidgetControllerLibrary.h"

AAuraProjectile::AAuraProjectile()
{
 
	
	PrimaryActorTick.bCanEverTick = false;
	Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	SetRootComponent(Sphere);
	
	Sphere->SetCollisionObjectType(ECC_Projectile);
	Sphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Sphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	Sphere->SetCollisionResponseToChannel(ECC_WorldDynamic,ECR_Overlap);
	Sphere->SetCollisionResponseToChannel(ECC_WorldStatic,ECR_Overlap);
	Sphere->SetCollisionResponseToChannel(ECC_Pawn,ECR_Overlap);
	bReplicates = true;
	SetReplicateMovement(true);
	
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");
	ProjectileMovement->InitialSpeed = 550.f;
	ProjectileMovement->MaxSpeed = 550.f;
	ProjectileMovement->ProjectileGravityScale = 0.f;
}


void AAuraProjectile::BeginPlay()
{
	Super::BeginPlay();
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AAuraProjectile::OnSphereOverlap);
	LoopPtr = UGameplayStatics::SpawnSoundAttached(LoopSound,GetRootComponent());
	SetLifeSpan(LifeSpan);
	
}

void AAuraProjectile::OnHit()
{
	UGameplayStatics::PlaySoundAtLocation(this,ImpactSound,GetActorLocation(),FRotator::ZeroRotator);
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactEffect, GetActorLocation());
	if (LoopPtr)
	{
		LoopPtr->Stop();
		LoopPtr->DestroyComponent();
	}
	IsHit = true;
}

void AAuraProjectile::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                      UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
	if (!IsvalidOverlap(OtherActor)) return;
	
	// DamageEffectParams 只存在于服务器生成的投射物上
	if (!HasAuthority())
	{
		IsHit = true;
		return;
	}

	UGameplayStatics::PlaySoundAtLocation(this,ImpactSound,GetActorLocation());
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactEffect, GetActorLocation());
	if (LoopPtr)
	{
		LoopPtr->Stop();
		
	}

	if (HasAuthority())
	{
		if (UAbilitySystemComponent* TargetAsc = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
		{
			DamageEffectParams.TargetAbilitySystemComponent = TargetAsc;
			UAuraWidgetControllerLibrary::ApplyDamageEffect(DamageEffectParams);
		}
		
		Destroy();
	}
	else
	{
		IsHit = true;
	}
}

void AAuraProjectile::Destroyed()
{
	if (!IsHit&&!HasAuthority())
	{
		UGameplayStatics::PlaySoundAtLocation(this,ImpactSound,GetActorLocation());
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactEffect, GetActorLocation());
		if (LoopPtr)
		{
			LoopPtr->Stop();
		}
		
	}
	
	Super::Destroyed();
}

bool AAuraProjectile::IsvalidOverlap(AActor* OtherActor)
{
	if (!OtherActor) return false;
	UAbilitySystemComponent* SourceASC = DamageEffectParams.SourceAbilitySystemComponent;
	if (!IsValid(SourceASC)){Destroy();return false;}
	AActor* SourceActor = SourceASC->GetAvatarActor();
	if (!IsValid(SourceActor) || SourceActor == OtherActor)return false;
	if (!UAuraWidgetControllerLibrary::ISBothFirend(SourceActor, OtherActor))return false;
	
	return true;
}


