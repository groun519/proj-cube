// Fill out your copyright notice in the Description page of Project Settings.


#include "P_Cube/UI/WidgetController/CubeWidgetController.h"

void UCubeWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& WCParams)
{
	PlayerController = WCParams.PlayerController;
	PlayerState = WCParams.PlayerState;
	AbilitySystemComponent = WCParams.AbilitySystemComponent;
	AttributeSet = WCParams.AttributeSet;
}

void UCubeWidgetController::BroadcastInitialValues()
{

}

void UCubeWidgetController::BindCallbacksToDependencies()
{

}
