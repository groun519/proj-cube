// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "CubeGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class P_CUBE_API UCubeGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
public:

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	FGameplayTag StartupInputTag;

	virtual FString GetBasicDescription(int32 Level);
	virtual FString GetUniqueDescription(int32 Level);
	virtual FString GetDetailedBasicDescription(int32 Level);
	virtual FString GetDetailedUniqueDescription(int32 Level);

protected:

	float GetManaCost(float InLevel = 1.f) const;
	float GetCooldown(float InLevel = 1.f) const;
};
