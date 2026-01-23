#pragma once

#include "CoreMinimal.h"
#include "ScalableFloat.h"
#include "QVProjectileTypes.generated.h"

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