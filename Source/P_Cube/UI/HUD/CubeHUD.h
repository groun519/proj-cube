// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "CubeHUD.generated.h"

class UAttributeSet;
class UAbilitySystemComponent;
class UOverlayWidgetController;
class UCubeUserWidget;
struct FWidgetControllerParams;

/**
 * 
 */
UCLASS()
class P_CUBE_API ACubeHUD : public AHUD
{
	GENERATED_BODY()
	
public:

	UPROPERTY()
	TObjectPtr<UCubeUserWidget>  OverlayWidget;

	UOverlayWidgetController* GetOverlayWidgetController(const FWidgetControllerParams& WCParams);

	void InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS);

protected:

private:

	UPROPERTY(EditAnywhere)
	TSubclassOf<UCubeUserWidget> OverlayWidgetClass;

	UPROPERTY()
	TObjectPtr<UOverlayWidgetController> OverlayWidgetController;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UOverlayWidgetController> OverlayWidgetControllerClass;
};
