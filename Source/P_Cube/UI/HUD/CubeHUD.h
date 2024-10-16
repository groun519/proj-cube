// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "CubeHUD.generated.h"

class UAttributeMenuWidgetController;
class UAttributeSet;
class UAbilitySystemComponent;
class UOverlayWidgetController;
class UCubeUserWidget;
struct FWidgetControllerParams;
class USkillMenuWidgetController;

/**
 * 
 */
UCLASS()
class P_CUBE_API ACubeHUD : public AHUD
{
	GENERATED_BODY()
	
public:

	UOverlayWidgetController* GetOverlayWidgetController(const FWidgetControllerParams& WCParams);
	UAttributeMenuWidgetController* GetAttributeMenuWidgetController(const FWidgetControllerParams& WCParams);
	USkillMenuWidgetController* GetSkillMenuWidgetController(const FWidgetControllerParams& WCParams);

	void InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS);

protected:

private:

	UPROPERTY()
	TObjectPtr<UCubeUserWidget>  OverlayWidget;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UCubeUserWidget> OverlayWidgetClass;

	UPROPERTY()
	TObjectPtr<UOverlayWidgetController> OverlayWidgetController;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UOverlayWidgetController> OverlayWidgetControllerClass;

	UPROPERTY()
	TObjectPtr<UAttributeMenuWidgetController> AttributeMenuWidgetController;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UAttributeMenuWidgetController> AttributeMenuWidgetControllerClass;

	UPROPERTY()
	TObjectPtr<USkillMenuWidgetController> SkillMenuWidgetController;

	UPROPERTY(EditAnywhere)
	TSubclassOf<USkillMenuWidgetController> SkillMenuWidgetControllerClass;
};
