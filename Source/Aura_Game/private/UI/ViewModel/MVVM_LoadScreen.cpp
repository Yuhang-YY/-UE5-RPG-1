// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ViewModel/MVVM_LoadScreen.h"

#include "UI/ViewModel/MVVM_LoadSlot.h"

UMVVM_LoadSlot* UMVVM_LoadScreen::GetLoadSlotViewModel(int32 Index)
{
	return LoadSlotViewModelMap.FindChecked(Index);
}

void UMVVM_LoadScreen::InitializeLoadSlotViewModel()
{
	LoadSlot_0 = NewObject<UMVVM_LoadSlot>(LoadSlotViewModelClass);
	LoadSlotViewModelMap.Add(0,LoadSlot_0);
	
	LoadSlot_1 = NewObject<UMVVM_LoadSlot>(LoadSlotViewModelClass);
	LoadSlotViewModelMap.Add(1,LoadSlot_1);
	
	LoadSlot_2= NewObject<UMVVM_LoadSlot>(LoadSlotViewModelClass);
	LoadSlotViewModelMap.Add(2,LoadSlot_2);
}
