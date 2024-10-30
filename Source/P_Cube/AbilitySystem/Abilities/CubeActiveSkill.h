// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CubeDamageGameplayAbility.h"
#include "CubeActiveSkill.generated.h"

class ACubeProjectile;
class ACubeHitbox;
class UGameplayEffect;
struct FGameplayTag;

/**
 * 
 */
UCLASS()
class P_CUBE_API UCubeActiveSkill : public UCubeDamageGameplayAbility
{
	GENERATED_BODY()
protected:

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UFUNCTION(BlueprintCallable, Category = "Projectile")
	void SpawnProjectile(const FName ProjectileName, const FName DamageName, const FVector& ProjectileTargetLocation, const FGameplayTag& SocketTag, bool bOverridePitch = false, float PitchOverride = 0.f, bool bOverrideYaw = false, float YawOverride = 0.f, AActor* InstigatorPlayer = nullptr, bool bIsOnlyAttackTargetActor = false, AActor* TargetActor = nullptr);

	UFUNCTION(BlueprintCallable, Category = "Hitbox")
	void SpawnHitBox(const FName HitboxName, const FName DamageName, const FTransform& HitboxTransform, AActor* InstigatorPlayer = nullptr, AActor* TargetActor = nullptr);



	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<FName, TSubclassOf<ACubeProjectile>> ProjectileClassMap;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<FName, TSubclassOf<ACubeHitbox>> HitboxClassMap;
};
