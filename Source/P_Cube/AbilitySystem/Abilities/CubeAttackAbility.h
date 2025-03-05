// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "P_Cube/AbilitySystem/Abilities/CubeActiveSkill.h"
#include "UObject/NoExportTypes.h"
#include "CubeAttackAbility.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_DELEGATE(FOnTickEvent);

UCLASS()
class P_CUBE_API UCubeAttackAbility : public UCubeActiveSkill
{
	GENERATED_BODY()
	
	UCubeAttackAbility();

public:

	/*UFUNCTION(BlueprintCallable, Category = "AttackSystem")
	void AttackCycle(AActor* AttackTarget, UAnimMontage* AttackMontage, float Range, float AttackSpeed, float PreDelay, float PostDelay);*/

	UPROPERTY(BlueprintReadWrite, Category = "AttackSystem")
	bool bAttackLoopActive;

	UPROPERTY(BlueprintReadWrite, Category = "AttackSystem")
	float Distance;

	UPROPERTY(BlueprintReadWrite, Category = "AttackSystem")
	float Range;

	virtual void CancelAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateCancelAbility
	) override;

	UFUNCTION(BlueprintCallable, Category = "AttackSystem")
	void StartTickTimer(
		const FOnTickEvent& TickEvent, 
		AActor* NewAttackTarget
		);
		
protected:
	//bool PlayAttackMontage(UAnimMontage* AttackMontage, float Rate);

private:
	FOnTickEvent StoredTickEvent;

	void OnTick();

	FTimerHandle TickTimerHandle;
};
