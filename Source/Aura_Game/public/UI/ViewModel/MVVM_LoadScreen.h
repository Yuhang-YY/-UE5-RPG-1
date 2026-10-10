// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "MVVM_LoadScreen.generated.h"

class UMVVM_LoadSlot;

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSelectedButton);

UCLASS()
class AURA_GAME_API UMVVM_LoadScreen : public UMVVMViewModelBase
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintPure)
	UMVVM_LoadSlot* GetLoadSlotViewModel(int32 Index);
	
	void InitializeLoadSlotViewModel();
	
	UFUNCTION(BlueprintCallable) //当Taken按下时调用的函数
	void NewSlotButtonPressed(int32 Index,const FString& PlayerName);
	
	UFUNCTION(BlueprintCallable)
	void NewGameButtonPressed(int32 Index);
	
	UFUNCTION(BlueprintCallable)
	void SelectionSlotButtonPressed(int32 Index);
	
	UFUNCTION(BlueprintCallable)
	void SureDeleteButtonPressed();
	
	UFUNCTION(BlueprintCallable)
	void PlayButtonPressed();
	
	void LoadData();
	
	UPROPERTY(BlueprintAssignable)
	FSelectedButton SelectedButton;
private:
	UPROPERTY()
	UMVVM_LoadSlot* SelectedSlot;
	
	UPROPERTY()
	TMap<int32, UMVVM_LoadSlot*> LoadSlotViewModelMap;
	
	UPROPERTY()
	TObjectPtr<UMVVM_LoadSlot> LoadSlot_0;
	
	UPROPERTY()
	TObjectPtr<UMVVM_LoadSlot> LoadSlot_1;
	
	UPROPERTY()
	TObjectPtr<UMVVM_LoadSlot> LoadSlot_2;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UMVVM_LoadSlot> LoadSlotViewModelClass;
};
