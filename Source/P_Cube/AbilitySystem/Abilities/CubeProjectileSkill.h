// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CubeDamageGameplayAbility.h"
#include "CubeProjectileSkill.generated.h"

class ACubeProjectile;
class UGameplayEffect;

/**
 * 
 */
UCLASS()
class P_CUBE_API UCubeProjectileSkill : public UCubeDamageGameplayAbility
{
	GENERATED_BODY()
protected:

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UFUNCTION(BlueprintCallable, Category = "Projectile")
	void SpawnProjectile(const FVector& ProjectileTargetLocation);

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<ACubeProjectile> ProjectileClass;
};
