// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameplayTagContainer.h"
#include "CubePlayerController.generated.h"

// 전방선언
class IHighlightInterface;
class UNiagaraSystem;
class UDamageTextComponent;
class UInputMappingContext; 
class UInputAction;
struct FInputActionValue;
class UCubeInputConfig;
class UCubeAbilitySystemComponent;
class USplineComponent;
class ADecalEffectActor;

enum class ETargetingStatus : uint8
{
	TargetingEnemy,
	TargetingNonEnemy,
	NotTargeting
};

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

	bool bClickRightMouse;

	bool HaveTarget;

	AActor* Target;
	FVector TargetVec;

	UFUNCTION(Client, Reliable)
	void ShowDamageNumber(float DamageAmount, ACharacter* TargetCharacter, bool bCriticalHit, bool bPhysicalHit, bool bMagicalHit, bool bPureHit, bool bHealHit);

	UFUNCTION(BlueprintCallable)
	void ShowDecalEffectActor(FVector Scale, UMaterialInterface* DecalMaterial = nullptr);

	UFUNCTION(BlueprintCallable)
	void HideDecalEffectActor();

	void SetPathAndRun(FVector Destination);

protected:
	virtual void BeginPlay();
	virtual void SetupInputComponent() override;

private:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> CubeContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> InputMouseRight;

	void CursorTrace();

	TObjectPtr<AActor> LastActor;
	TObjectPtr<AActor> ThisActor;
	FHitResult CursorHit;
	static void HighlightActor(AActor* InActor);
	static void UnHighlightActor(AActor* InActor);

	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UCubeInputConfig> InputConfig;

	UPROPERTY()
	TObjectPtr<UCubeAbilitySystemComponent> CubeAbilitySystemComponent;

	UCubeAbilitySystemComponent* GetASC();


	// 클릭 이동 변수
	FVector CachedDestination = FVector::ZeroVector; // 목표 벡터
	float FollowTime = 0.f; //
	float ShortPressThreshold = 0.5f; 
	bool bAutoRunning = false; // 
	bool bAutoAttack = false; // 
	ETargetingStatus TargetingStatus = ETargetingStatus::NotTargeting;

	UPROPERTY(EditDefaultsOnly)
	float AutoRunAcceptanceRadius = 50.f;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USplineComponent> Spline; // 경로 곡선

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UNiagaraSystem> ClickNiagaraSystem;

	void AutoRun();

	void MoveToInteractable(AActor* InteractableActor);
	void CheckInteraction();
	FTimerHandle CheckInteractionHandle;  
	AActor* TargetInteractable = nullptr;  


	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UDamageTextComponent> DamageTextComponentClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ADecalEffectActor> DecalEffectActorClass;

	UPROPERTY()
	TObjectPtr<ADecalEffectActor> DecalEffectActor;

	void UpdateDecalEffectActorLocation();
};
