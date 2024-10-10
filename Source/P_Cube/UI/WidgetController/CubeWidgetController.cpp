// Fill out your copyright notice in the Description page of Project Settings.


#include "P_Cube/UI/WidgetController/CubeWidgetController.h"

#include "P_Cube/Player/CubePlayerController.h"
#include "P_Cube/Player/CubePlayerState.h"
#include "P_Cube/AbilitySystem/CubeAbilitySystemComponent.h"
#include "P_Cube/AbilitySystem/CubeAttributeSet.h"
#include "P_Cube/AbilitySystem/Data/AbilityInfo.h"

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

void UCubeWidgetController::BroadcastAbilityInfo()
{
	if (!GetCubeASC()->bStartupAbilitiesGiven) return;
	FForEachAbility BroadcastDelegate;
	BroadcastDelegate.BindLambda([this](const FGameplayAbilitySpec& AbilitySpec)
		{
			FCubeAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(CubeAbilitySystemComponent->GetAbilityTagFromSpec(AbilitySpec));
			Info.InputTag = CubeAbilitySystemComponent->GetInputTagFromSpec(AbilitySpec);
			Info.StatusTag = CubeAbilitySystemComponent->GetStatusFromSpec(AbilitySpec);
			AbilityInfoDelegate.Broadcast(Info);
		});
	GetCubeASC()->ForEachAbility(BroadcastDelegate);
}

ACubePlayerController* UCubeWidgetController::GetCubePC()
{
	if (CubePlayerController == nullptr)
	{
		CubePlayerController = Cast<ACubePlayerController>(PlayerController);
	}
	return CubePlayerController;
}

ACubePlayerState* UCubeWidgetController::GetCubePS()
{
	if (CubePlayerState == nullptr)
	{
		CubePlayerState = Cast<ACubePlayerState>(PlayerState);
	}
	return CubePlayerState;
}

UCubeAbilitySystemComponent* UCubeWidgetController::GetCubeASC()
{
	if (CubeAbilitySystemComponent == nullptr)
	{
		CubeAbilitySystemComponent = Cast<UCubeAbilitySystemComponent>(AbilitySystemComponent);
	}
	return CubeAbilitySystemComponent;
}

UCubeAttributeSet* UCubeWidgetController::GetCubeAS()
{
	if (CubeAttributeSet == nullptr)
	{
		CubeAttributeSet = Cast<UCubeAttributeSet>(AttributeSet);
	}
	return CubeAttributeSet;
}
