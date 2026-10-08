// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "MVVM_LoadSlot.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSetWidgetSwitcherIndex,int32,WidgetSwitcherIndex);
/**
 * 
 */
UCLASS()
class AURA_GAME_API UMVVM_LoadSlot : public UMVVMViewModelBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintAssignable)
	FSetWidgetSwitcherIndex SetWidgetSwitcherIndex;
	
	void InitializeSlot();

	
	
	
	void SetLoadSlotName(FString InLoadSlotName);
	FString GetLoadSlotName()const{ return LoadSlotName;}

private:
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,FieldNotify,Setter,Getter,meta=(AllowPrivateAccess=true));
	FString LoadSlotName;
};
