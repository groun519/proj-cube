// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "P_Cube/AbilitySystem/Abilities/CubeGameplayAbility.h"
#include "CubeDamageGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class P_CUBE_API UCubeDamageGameplayAbility : public UCubeGameplayAbility
{
	GENERATED_BODY()
protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	TMap<FGameplayTag, FScalableFloat> DamageTypes;
};
