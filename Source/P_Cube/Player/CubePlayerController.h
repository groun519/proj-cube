// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CubePlayerController.generated.h"

class UInputMappingContext; // 전방선언
class UInputAction;
struct FInputActionValue;
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
	virtual void PlayerTick(float DeltaTime) override;

	void MouseRightPressed(const FInputActionValue& Value);
	void MouseRightCompleted(const FInputActionValue& Value);

	bool bClickRightMouse;

	bool HaveTarget;

	AActor* Target;
	FVector TargetVec;

	class ACubeCharacter* Character; // <<

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
