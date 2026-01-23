// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "P_Cube/AbilitySystem/Abilities/CubeActiveSkill.h"
#include "Longbow_Basic.generated.h"

/**
 * 
 */
UCLASS()
class P_CUBE_API ULongbow_Basic : public UCubeActiveSkill
{
	GENERATED_BODY()
	
public:

	virtual FString GetDescription(int32 Level, bool bIsDetailed) override;

	/*UFUNCTION(BlueprintCallable)
	void SpawnProjectiles(const FVector& ProjectileTargetLocation, const FGameplayTag& SocketTag, bool bOverridePitch, float PitchOverride, AActor* HomingTarget);*/

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Longbow_Basic")
	float ProjectileSpread = 90.f;

	UPROPERTY(EditDefaultsOnly, Category = "Longbow_Basic")
	int32 MaxNumProjectiles = 5;
};
