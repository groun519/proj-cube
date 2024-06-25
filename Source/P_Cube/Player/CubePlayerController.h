// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "InputMappingContext.h"
#include "P_Cube/InputConfigData.h"
#include "CubePlayerController.generated.h"

class IEnemyInterface;

/**
 *
 */
UCLASS()
class P_CUBE_API ACubePlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	ACubePlayerController();

	void MouseRightPressed(const FInputActionValue& Value);
	void MouseRightCompleted(const FInputActionValue& Value);

	/* Reference to the UMG asset in the editor */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
		TSubclassOf<class UUserWidget> HUDOverlayAsset;

	/* Variable to hold the widget after creating it */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
		UUserWidget* HUDOverlay;

	virtual void PlayerTick(float DeltaTime) override;

	bool bClickRightMouse;

	bool HaveTarget;

	AActor* Target;
	FVector TargetVec;

	class ACubeCharacter* Character; // <<

	//void SetNewDestination(const FVector Destionation);

	void MoveToMouseCursor();

	void TraceHitTarget();

	void SetTarget();

protected:
	virtual void BeginPlay();
	virtual void SetupInputComponent() override;

private:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> CubeContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> InputMouseRight;

	void CursorTrace();

	TScriptInterface<IEnemyInterface> LastActor;
	TScriptInterface<IEnemyInterface> ThisActor;
};
