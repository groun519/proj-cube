// Fill out your copyright notice in the Description page of Project Settings.


#include "CubeAbilitySystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "P_Cube/UI/WidgetController/CubeWidgetController.h"
#include "P_Cube/Player/CubePlayerState.h"
#include "P_Cube/UI/HUD/CubeHUD.h"

UOverlayWidgetController* UCubeAbilitySystemLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (ACubeHUD* CubeHUD = Cast<ACubeHUD>(PC->GetHUD()))
		{
			ACubePlayerState* PS = PC->GetPlayerState<ACubePlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();
			const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
			return CubeHUD->GetOverlayWidgetController(WidgetControllerParams);
		}
	}
	return nullptr;
}

UAttributeMenuWidgetController* UCubeAbilitySystemLibrary::GetAttributeMenuWidgetController(const UObject* WorldContextObject)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (ACubeHUD* CubeHUD = Cast<ACubeHUD>(PC->GetHUD()))
		{
			ACubePlayerState* PS = PC->GetPlayerState<ACubePlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();
			const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
			return CubeHUD->GetAttributeMenuWidgetController(WidgetControllerParams);
		}
	}
	return nullptr;
}
