// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/AuraPlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "UAuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Actor/MagicCircle.h"
#include "Aura_Game/Aura_Game.h"
#include "Components/DecalComponent.h"
#include "Components/SplineComponent.h"
#include "GameFramework/Character.h"
#include "Input/AuraInputComponent.h"
#include "Interaction/EnemyInterface.h"
#include "UI/WidgetComponent/DamageTextComponent.h"


AAuraPlayerController::AAuraPlayerController()
{
	//将此控制器设置为可复制的实体‘
	bReplicates = true;
	Spline = CreateDefaultSubobject<USplineComponent>("Spline");
}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();
	check(AuraContext);
	
	//通过本地用户获取增强输入本地子系统，并添加绑定输入映射上下文
	UEnhancedInputLocalPlayerSubsystem * Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (Subsystem)
	{
		Subsystem->AddMappingContext(AuraContext, 0);
	}
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
	
	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);
	
}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	
	CursorTrace();
	AutoMoveing();
	UpdateMagicCircleLocation();
}

void AAuraPlayerController::ShowMagicCircle(UMaterialInterface* DecalMaterial)
{
	if (!IsValid(MagicCircle))
	{
		MagicCircle = GetWorld()->SpawnActor<AMagicCircle>(MagicCircleClass);
		if (DecalMaterial != nullptr)
		{
			MagicCircle->DecalComponent->SetMaterial(0, DecalMaterial);
		}
		SetShowMouseCursor(false);
	}

}

void AAuraPlayerController::HideMagicCircle()
{
	if (IsValid(MagicCircle))
	{
		MagicCircle->Destroy();
		SetShowMouseCursor(true);
	}
}


void AAuraPlayerController::ShowDamageText_Implementation(float Damage,ACharacter* Chara,bool IsBlock,bool IsCritical)
{
	if (DamageWidgetComponent)
	{
		UDamageTextComponent* DamageTextComponent = NewObject<UDamageTextComponent>(Chara,DamageWidgetComponent);
		DamageTextComponent->RegisterComponent();
		DamageTextComponent->AttachToComponent(Chara->GetRootComponent(),FAttachmentTransformRules::KeepRelativeTransform);
		DamageTextComponent->SetWidgetSpace(EWidgetSpace::Screen);
		DamageTextComponent->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		
		DamageTextComponent->SetDamageText(Damage,IsBlock,IsCritical);
	}
}

void AAuraPlayerController::AutoMoveing()
{
	if (AutoRunning)
	{
		if (APawn* PawnController = GetPawn())
		{
			const FVector LocationToSpline = Spline->FindLocationClosestToWorldLocation(PawnController->GetActorLocation(),ESplineCoordinateSpace::World);
			const FVector Direction = Spline->FindDirectionClosestToWorldLocation(LocationToSpline,ESplineCoordinateSpace::World);
		
			PawnController->AddMovementInput(Direction);
			
			const float Local = (LocationToSpline - CachedDestination).Length();
			if (Local<=AutoRunAcceptanceRadius)
			{
				AutoRunning = false;
			}
		}
	}
	
}


void AAuraPlayerController::CursorTrace()
{
	
	if (GetASC() && GetASC()->HasMatchingGameplayTag(FUAuraGameplayTags::Get().Player_Block_CursorTrace))
	{
		if (LastAction)
		{
			LastAction->UnHighlightActor();
		}
		if (ThisAction)
		{
			ThisAction->UnHighlightActor();
		}
		
		LastAction = nullptr;
		ThisAction = nullptr;
		return;
	}
	
	ECollisionChannel Channel = IsValid(MagicCircle)? ECC_ExcludePlayers:ECC_Visibility;
	GetHitResultUnderCursor(Channel,false,Hit);
	if (!Hit.bBlockingHit) {return;}
	
	//一个记录上一个的物体，防止this为空
	LastAction = ThisAction;
	ThisAction = Cast<IEnemyInterface>(Hit.GetActor());
	
	/**
	 *   命中物体情况
	 *   1.LastAction 和 ThisActor 都为nullptr 此时什么也不做                   什么都没点
	 *   2、LastAction有效，但ThisActor无效 此时LastActor调用UnHighlight函数     上次移到敌人了这次没移到
	 *   3.thisActor有效，但是Last无效，此时thisactor调用Higlight函数            这次移到了上次没移到
	 *   4.两个都有效 但是Last!=This    Last->UnHigtlight    this->higtlight   这次移到的敌人与上次不一样
	 *   5.两个都有效，且是同一个敌人   什么都不用做                               两个都有效，且是同一个敌人
	 * 
	 */
	
	if (LastAction!=ThisAction)
	{
		if (LastAction)
		{
			LastAction->UnHighlightActor();
		}
		if (ThisAction)
		{
			ThisAction->HighlightActor();
		}
	}
	
}

void AAuraPlayerController::UpdateMagicCircleLocation()
{
	if (IsValid(MagicCircle))
	{
		MagicCircle->SetActorLocation(Hit.ImpactPoint);
	}
}


void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	//创建增强输入组件
	
	UAuraInputComponent* AuraInputComponent = CastChecked<UAuraInputComponent>(InputComponent);
	AuraInputComponent->BindAction(MoveAction,ETriggerEvent::Triggered,this,&AAuraPlayerController::Move);
	AuraInputComponent->BindAction(ShiftAction,ETriggerEvent::Started,this,&AAuraPlayerController::ShiftPressed);
	AuraInputComponent->BindAction(ShiftAction,ETriggerEvent::Completed,this,&AAuraPlayerController::ShiftReleased);
	//所有的输入参数根据数据资产绑定了对应的操作
	AuraInputComponent->BindAbilityActions(InputConfig,this,&ThisClass::AbilityInputPress,&ThisClass::AbilityInputRelease,&ThisClass::AbilityInputHeld);
	
}

void AAuraPlayerController::Move(const FInputActionValue& Value)
{
	if (GetASC() && GetASC()->HasMatchingGameplayTag(FUAuraGameplayTags::Get().Player_Block_InputPressed))
	{
		return;
	}
	
	FVector2D MovementVector = Value.Get<FVector2D>();
	
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);
	
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	
	APawn * pawn = GetPawn<APawn>();
	pawn->AddMovementInput(ForwardDirection, MovementVector.Y);
	pawn->AddMovementInput(RightDirection, MovementVector.X);
}

void AAuraPlayerController::AbilityInputPress(FGameplayTag InputTag)
{
	if (GetASC() && GetASC()->HasMatchingGameplayTag(FUAuraGameplayTags::Get().Player_Block_InputPressed))
	{
		return;
	}
	
	if (InputTag.MatchesTagExact(FUAuraGameplayTags::Get().InputTag_LMB))
	{
		bIsCursorTracing = ThisAction? true : false;
		AutoRunning = false ;
	}
	if (GetASC()) GetASC()->AbilityInputPress(InputTag);
}

void AAuraPlayerController::AbilityInputRelease(FGameplayTag InputTag)
{
	if (GetASC() && GetASC()->HasMatchingGameplayTag(FUAuraGameplayTags::Get().Player_Block_InputReleased))
	{
		return;
	}
	
	if (!InputTag.MatchesTagExact(FUAuraGameplayTags::Get().InputTag_LMB))
	{
		if (GetASC())
		{
			GetASC()->AbilityInputTagRelease(InputTag);
			return;
		}
	}
	if (GetASC())
	{
		GetASC()->AbilityInputTagRelease(InputTag);
	}
	if (!bIsCursorTracing&&!IsShiftDown)
	{
		APawn* PawnController = GetPawn();
		if (FollowTime <= ShortPressThreshold && PawnController)
		{
			if (UNavigationPath * Path = UNavigationSystemV1::FindPathToLocationSynchronously(this,PawnController->GetActorLocation(),CachedDestination))
			{
				Spline->ClearSplinePoints();
				for (const FVector& PathLoc : Path->PathPoints)
				{
					Spline->AddSplinePoint(PathLoc,ESplineCoordinateSpace::World);
				}
				if (Path->PathPoints.Num() > 0)
				{
					CachedDestination = Path->PathPoints[Path->PathPoints.Num() - 1];
					AutoRunning = true;
				}
				if (GetASC() && !GetASC()->HasMatchingGameplayTag(FUAuraGameplayTags::Get().Player_Block_InputPressed))
				{
					UNiagaraFunctionLibrary::SpawnSystemAtLocation(this,ClinkNiagaraSystem,CachedDestination);
				}
				
			}
		}
		FollowTime = 0.f;
		bIsCursorTracing = false;
	}
}

void AAuraPlayerController::AbilityInputHeld(FGameplayTag InputTag)
{
	if (GetASC() && GetASC()->HasMatchingGameplayTag(FUAuraGameplayTags::Get().Player_Block_InputHeld))
	{
		return;
	}
	
	if (!InputTag.MatchesTagExact(FUAuraGameplayTags::Get().InputTag_LMB))
	{
		if (GetASC())
		{
			GetASC()->AbilityInputTagHeld(InputTag);
			return;
		}
	}
	
	if (bIsCursorTracing||IsShiftDown)
	{
		if (GetASC())
		GetASC()->AbilityInputTagHeld(InputTag);
	}
	else
	{
		FollowTime += GetWorld()->GetDeltaSeconds();
		
		if (Hit.bBlockingHit)
		{
			CachedDestination = Hit.Location;
		}
		
		if (APawn* PawnController = GetPawn())
		{
			const FVector WorldDirection = (CachedDestination - PawnController->GetActorLocation()).GetSafeNormal();
			PawnController->AddMovementInput(WorldDirection);
		}
	}
	
}

UAuraAbilitySystemComponent* AAuraPlayerController::GetASC()
{
	if (AbilitySystemComponent == nullptr)
	{
		AbilitySystemComponent =Cast<UAuraAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));
	}
	return AbilitySystemComponent;
}


