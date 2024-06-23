// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "InputMappingContext.h"
#include "P_Cube/InputConfigData.h"
#include "CubePlayerController.generated.h"

/**
 *
 */
UCLASS()
class P_CUBE_API ACubePlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	ACubePlayerController(const FObjectInitializer& ObjectInitalizer =	FObjectInitializer::Get());

public:
	void MouseRightPressed(const FInputActionValue& Value);
	void MouseRightCompleted(const FInputActionValue& Value);

public:
	/* Reference to the UMG asset in the editor */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
		TSubclassOf<class UUserWidget> HUDOverlayAsset;

	/* Variable to hold the widget after creating it */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
		UUserWidget* HUDOverlay;

protected:
	bool bClickRightMouse;

	bool HaveTarget;

	AActor* Target;
	FVector TargetVec;

	class ACubeCharacter* Character; // <<

	virtual void BeginPlay();

	//void SetNewDestination(const FVector Destionation);

	void MoveToMouseCursor();

	void TraceHitTarget();

	void SetTarget();

	virtual void SetupInputComponent() override;

	virtual void PlayerTick(float DeltaTime) override;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
		UInputMappingContext* InputMapping;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
		UInputAction* InputMouseRight;
};
