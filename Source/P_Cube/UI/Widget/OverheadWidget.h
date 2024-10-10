// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "P_Cube/UI/Widget/CubeUserWidget.h"
#include "OverheadWidget.generated.h"

/**
 * 
 */
UCLASS()
class P_CUBE_API UOverheadWidget : public UCubeUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* DisplayText;

	void SetDisplayText(FString TextToDisplay);

	UFUNCTION(BlueprintCallable)
	void ShowPlayerNetRole(APawn* InPawn);

	void SetPlayerNameText(FString PlayerName);

	UFUNCTION(BlueprintCallable)
	void ShowPlayerName(APawn* InPawn);

protected:
	void NativeDestruct();
};
