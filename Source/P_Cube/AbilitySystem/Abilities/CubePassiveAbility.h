// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "P_Cube/AbilitySystem/Passive/PassiveNiagaraComponent.h"
#include "P_Cube/AbilitySystem/Abilities/CubeDamageGameplayAbility.h"

#include "CubePassiveAbility.generated.h"

/**
 * 
 */
UCLASS()
class P_CUBE_API UCubePassiveAbility : public UCubeDamageGameplayAbility
{
	GENERATED_BODY()
	
public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
	void ReceiveDeactivate(const FGameplayTag& AbilityTag);

	TObjectPtr<UPassiveNiagaraComponent> PassiveNiagaraComp;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UNiagaraSystem> PassiveNiagara;

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag PassiveTag;
};
