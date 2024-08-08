// Fill out your copyright notice in the Description page of Project Settings.


#include "CubeAbilitySystemGlobals.h"
#include "P_Cube/CubeAbilityTypes.h"

FGameplayEffectContext* UCubeAbilitySystemGlobals::AllocGameplayEffectContext() const
{
	return new FCubeGameplayEffectContext();
}
