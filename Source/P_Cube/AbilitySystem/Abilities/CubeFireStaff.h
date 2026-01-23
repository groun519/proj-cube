// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "p_Cube/AbilitySystem/Abilities/CubeProjectileSkill.h"
#include "CubeFireStaff.generated.h"

/**
 * 
 */
UCLASS()
class P_CUBE_API UCubeFireStaff : public UCubeProjectileSkill
{
	GENERATED_BODY()
	
public:

	virtual FString GetDescription(int32 Level, bool  bIsDetailed) override;
};
