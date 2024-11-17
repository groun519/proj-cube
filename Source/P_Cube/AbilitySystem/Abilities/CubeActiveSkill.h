// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CubeDamageGameplayAbility.h"
#include "CubeActiveSkill.generated.h"

class ACubeProjectile;
class ACubeHitbox;
class UGameplayEffect;
struct FGameplayTag;

USTRUCT(BlueprintType)
struct FMultipleProjectilesFeacher
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	bool bUseMultipleProjectilesFeacher = false;

	UPROPERTY(BlueprintReadWrite)
	float ProjectileSpread = 90.f;

	UPROPERTY(BlueprintReadWrite)
	int32 NumProjectiles = 1;

	UPROPERTY(BlueprintReadWrite)
	int32 MaxNumProjectiles = 1;

	UPROPERTY(BlueprintReadWrite)
	int32 UpgradeProjectiles = 1;
};

USTRUCT(BlueprintType)
struct FHomingFeacher
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	bool bUseHomingFeacher = false;

	UPROPERTY(BlueprintReadWrite)
	float HomingAccelerationMin = 1600.f;

	UPROPERTY(BlueprintReadWrite)
	float HomingAccelerationMax = 3200.f;

	UPROPERTY(BlueprintReadWrite)
	AActor* HomingTarget = nullptr;
};

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
	void SpawnProjectile(
		const FName ProjectileName, 
		const FName DamageName, 
		const FVector& ProjectileTargetLocation, 
		const FGameplayTag& SocketTag, 
		bool bOverridePitch = false, 
		float PitchOverride = 0.f, 
		bool bOverrideYaw = false, 
		float YawOverride = 0.f, 
		AActor* InstigatorPlayer = nullptr,
		bool bIsOnlyAttackTargetActor = false, 
		AActor* TargetActor = nullptr,
		FMultipleProjectilesFeacher MultipleProjectilesFeacher = FMultipleProjectilesFeacher(),
		FHomingFeacher HomingFeacher = FHomingFeacher()
	);

	UFUNCTION(BlueprintCallable, Category = "Hitbox")
	void SpawnHitBox(
		const FName HitboxName, 
		const FName DamageName, 
		const FTransform& HitboxTransform, 
		const bool bOnlyPlayer, 
		AActor* InstigatorPlayer = nullptr, 
		AActor* TargetActor = nullptr
	);

	/** Projectile Skill's Properties **/
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<FName, TSubclassOf<ACubeProjectile>> ProjectileClassMap;
	/** end Projectile Skill's Properties **/

	/** Hitbox Skill's Properties **/
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<FName, TSubclassOf<ACubeHitbox>> HitboxClassMap;
	/** end Hitbox Skill's Properties **/

private:
	UFUNCTION(BlueprintCallable)
	TArray<AActor*> FindNearestActorsByTag(const FName TagName, const FVector Location, const float Radius, const int32 findingPlayers, const bool bDrawDebugSphere);
};
