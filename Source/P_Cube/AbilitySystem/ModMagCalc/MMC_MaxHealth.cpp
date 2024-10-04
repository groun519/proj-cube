// Fill out your copyright notice in the Description page of Project Settings.


#include "P_Cube/AbilitySystem/ModMagCalc/MMC_MaxHealth.h"

#include "P_Cube/AbilitySystem/CubeAttributeSet.h"
#include "P_Cube/Interaction/CombatInterface.h"

UMMC_MaxHealth::UMMC_MaxHealth()
{
	
}

float UMMC_MaxHealth::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	// Gather tags from source and target
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	UObject* SourceObject = Spec.GetContext().GetSourceObject();

	ICombatInterface* CombatInterface = Cast<ICombatInterface>(Spec.GetContext().GetSourceObject());
	const int32 PlayerLevel = CombatInterface->Execute_GetPlayerLevel(SourceObject);

	return 925.f + 750.f * PlayerLevel;
}
