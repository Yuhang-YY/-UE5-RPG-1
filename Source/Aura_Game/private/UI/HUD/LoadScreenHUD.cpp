// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/LoadScreenHUD.h"

#include "Blueprint/UserWidget.h"
#include "UI/ViewModel/MVVM_LoadScreen.h"
#include "UI/Widget/LoadScreenWidget.h"

void ALoadScreenHUD::BeginPlay()
{
	Super::BeginPlay();
	
	//初始化主视图模型和三个插槽视图模型
	ViewModelLoadScreen = NewObject<UMVVM_LoadScreen>(this,ViewModelLoadScreenClass);
	ViewModelLoadScreen->InitializeLoadSlotViewModel();
	
	LoadScreenWidget = CreateWidget<ULoadScreenWidget>(GetWorld(),LoadScreenWidgetClass);
	LoadScreenWidget->AddToViewport();
	
	LoadScreenWidget->BlueprintInitializeWidget();
}
