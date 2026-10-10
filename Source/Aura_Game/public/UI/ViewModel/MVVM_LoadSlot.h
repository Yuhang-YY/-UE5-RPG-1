// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "MVVM_LoadSlot.generated.h"

enum ESaveSlotStatus : int;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSetWidgetSwitcherIndex, int32, WidgetSwitcherIndex);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEnableSelectedSlotButton, bool, Enable);
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
	
	UPROPERTY(BlueprintAssignable)
	FEnableSelectedSlotButton EnableSelectedSlotButton;
	
	void InitializeSlot();
	
	/*FieldNotifyFunction*/
	UFUNCTION()
	void SetPlayerName(const FString& InPlayerName);
	
	UFUNCTION()
	void SetMapName(const FString& InMapName);
	
	UFUNCTION()
	FString GetPlayerName()const{return PlayerName;}
	FString GetMapName()const{return MapName;}
	/*FieldNotifyFunction*/
	
	
	void SetLoadSlotName(FString InLoadSlotName);
	FString GetLoadSlotName()const{ return LoadSlotName;}

	UPROPERTY()
	FString LoadSlotName1;
	
	UPROPERTY()
	int32 LoadSlotIndex;
	
	UPROPERTY()
	TEnumAsByte<ESaveSlotStatus> SlotStatus;
	

private:
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,FieldNotify,Setter,Getter,meta=(AllowPrivateAccess=true));
	FString LoadSlotName;
	
	/*FieldNotifyProperty*/
	UPROPERTY(EditAnywhere,BlueprintReadOnly,FieldNotify,Setter,Getter,meta=(AllowPrivateAccess=true));
	FString PlayerName;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,FieldNotify,Setter,Getter,meta=(AllowPrivateAccess=true));
	FString MapName;
	/*FieldNotifyProperty*/
};
