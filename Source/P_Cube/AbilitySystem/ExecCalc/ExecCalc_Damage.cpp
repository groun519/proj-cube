// Fill out your copyright notice in the Description page of Project Settings.


#include "ExecCalc_Damage.h"

#include "AbilitySystemComponent.h"
#include "P_Cube/CubeAbilityTypes.h"
#include "P_Cube/CubeGameplayTags.h"
#include "P_Cube/AbilitySystem/CubeAbilitySystemLibrary.h"
#include "P_Cube/AbilitySystem/CubeAttributeSet.h"
#include "P_Cube/AbilitySystem/Data/CharacterClassInfo.h"
#include "P_Cube/Interaction/CombatInterface.h"

struct CubeDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArmorPenetration);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArmorPenetrationRate);

	DECLARE_ATTRIBUTE_CAPTUREDEF(MagicResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(MagicResistancePenetration);
	DECLARE_ATTRIBUTE_CAPTUREDEF(MagicResistancePenetrationRate);

	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalDamage);

	TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition> TagsToCaptureDefs;

	CubeDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UCubeAttributeSet, Armor, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UCubeAttributeSet, ArmorPenetration, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UCubeAttributeSet, ArmorPenetrationRate, Source, false);

		DEFINE_ATTRIBUTE_CAPTUREDEF(UCubeAttributeSet, MagicResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UCubeAttributeSet, MagicResistancePenetration, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UCubeAttributeSet, MagicResistancePenetrationRate, Source, false);

		DEFINE_ATTRIBUTE_CAPTUREDEF(UCubeAttributeSet, CriticalChance, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UCubeAttributeSet, CriticalDamage, Source, false);

		const FCubeGameplayTags& Tags = FCubeGameplayTags::Get();

		TagsToCaptureDefs.Add(Tags.Attributes_Primary_Armor, ArmorDef);
		TagsToCaptureDefs.Add(Tags.Attributes_Primary_ArmorPenetration, ArmorPenetrationDef);
		TagsToCaptureDefs.Add(Tags.Attributes_Primary_ArmorPenetrationRate, ArmorPenetrationRateDef);

		TagsToCaptureDefs.Add(Tags.Attributes_Primary_MagicResistance, MagicResistanceDef);
		TagsToCaptureDefs.Add(Tags.Attributes_Primary_MagicResistancePenetration, MagicResistancePenetrationDef);
		TagsToCaptureDefs.Add(Tags.Attributes_Primary_MagicResistancePenetrationRate, MagicResistancePenetrationRateDef);

		TagsToCaptureDefs.Add(Tags.Attributes_Primary_CriticalChance, CriticalChanceDef);
		TagsToCaptureDefs.Add(Tags.Attributes_Primary_CriticalDamage, CriticalDamageDef);
	}
};

static const CubeDamageStatics& DamageStatics()
{
	static CubeDamageStatics DStatics;
	return DStatics;
}

UExecCalc_Damage::UExecCalc_Damage()
{
	RelevantAttributesToCapture.Add(DamageStatics().ArmorDef);
	RelevantAttributesToCapture.Add(DamageStatics().ArmorPenetrationDef);
	RelevantAttributesToCapture.Add(DamageStatics().ArmorPenetrationRateDef);

	RelevantAttributesToCapture.Add(DamageStatics().MagicResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().MagicResistancePenetrationDef);
	RelevantAttributesToCapture.Add(DamageStatics().MagicResistancePenetrationRateDef);

	RelevantAttributesToCapture.Add(DamageStatics().CriticalChanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalDamageDef);
}

void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();

	AActor* SourceAvatar = SourceASC ? SourceASC->GetAvatarActor() : nullptr;
	AActor* TargetAvatar = TargetASC ? TargetASC->GetAvatarActor() : nullptr;
	ICombatInterface* SourceCombatInterface = Cast<ICombatInterface>(SourceAvatar);
	ICombatInterface* TargetCombatInterface = Cast<ICombatInterface>(TargetAvatar);

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	FGameplayEffectContextHandle EffectContextHandle = Spec.GetContext();

	// Set by Caller로 설정된 피해 값을 가져옴
	float Damage = 0.f;
	float SourcePenetration = 0.f;
	float SourcePenetrationRate = 0.f;
	int32 DamageType = 0;
	for (FGameplayTag DamageTypeTag : FCubeGameplayTags::Get().DamageTypes)
	{
		const float DamageTypeValue = Spec.GetSetByCallerMagnitude(DamageTypeTag);

		if (DamageTypeValue == 0) continue;

		if (DamageTypeTag.GetTagName() == TEXT("Damage.Physical"))
		{
			ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorPenetrationDef, EvaluationParameters, SourcePenetration);
			SourcePenetration = FMath::Max<float>(SourcePenetration, 0.f);

			ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorPenetrationRateDef, EvaluationParameters, SourcePenetrationRate);
			SourcePenetrationRate = FMath::Max<float>(SourcePenetrationRate, 0.f);

			DamageType = 0;
			UCubeAbilitySystemLibrary::SetIsPhysicalHit(EffectContextHandle, true);
		}
		else if (DamageTypeTag.GetTagName() == TEXT("Damage.Magical"))
		{
			ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().MagicResistancePenetrationDef, EvaluationParameters, SourcePenetration);
			SourcePenetration = FMath::Max<float>(SourcePenetration, 0.f);

			ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().MagicResistancePenetrationRateDef, EvaluationParameters, SourcePenetrationRate);
			SourcePenetrationRate = FMath::Max<float>(SourcePenetrationRate, 0.f);

			DamageType = 1;
			UCubeAbilitySystemLibrary::SetIsMagicalHit(EffectContextHandle, true);

		}
		else if (DamageTypeTag.GetTagName() == TEXT("Damage.Pure"))
		{
			DamageType = 2;
			UCubeAbilitySystemLibrary::SetIsPureHit(EffectContextHandle, true);
		}

		Damage += DamageTypeValue;
	}

	float TargetResistance = 0.f;
	float TargetResistanceRate = 0.f;
	if (DamageType == 0)
	{
		// 타겟의 Armor 값을 캡처하여 가져옴
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorDef, EvaluationParameters, TargetResistance);
		TargetResistance = FMath::Max<float>(TargetResistance, 0.f);

		// Armor 값을 기반으로 ArmorRate 값을 커브 테이블에서 가져옴
		const UCharacterClassInfo* CharacterClassInfo = UCubeAbilitySystemLibrary::GetCharacterClassInfo(SourceAvatar);
		const FRealCurve* ResistanceRateCurve = CharacterClassInfo->ResistanceRateCurveTable->FindCurve(FName("ArmorRate"), FString());
		TargetResistanceRate = ResistanceRateCurve->Eval(TargetResistance - SourcePenetration);
	}
	else if (DamageType == 1)
	{
		// 타겟의 MagicResistance 값을 캡처하여 가져옴
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().MagicResistanceDef, EvaluationParameters, TargetResistance);
		TargetResistance = FMath::Max<float>(TargetResistance, 0.f);

		// MagicResistance 값을 기반으로 MagicResistanceRate 값을 커브 테이블에서 가져옴
		const UCharacterClassInfo* CharacterClassInfo = UCubeAbilitySystemLibrary::GetCharacterClassInfo(SourceAvatar);
		const FRealCurve* ResistanceRateCurve = CharacterClassInfo->ResistanceRateCurveTable->FindCurve(FName("MagicResistanceRate"), FString());
		TargetResistanceRate = ResistanceRateCurve->Eval(TargetResistance - SourcePenetration);
	}
	
	/*/ " ArmorPenetation "
	float SourceArmorPenetration = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorPenetrationDef, EvaluationParameters, SourceArmorPenetration);
	SourceArmorPenetration = FMath::Max<float>(SourceArmorPenetration, 0.f);
	// " ArmorPenetationRate "
	float SourceArmorPenetrationRate = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorPenetrationRateDef, EvaluationParameters, SourceArmorPenetrationRate);
	SourceArmorPenetrationRate = FMath::Max<float>(SourceArmorPenetrationRate, 0.f); */

	const float EffectiveArmorRate = TargetResistanceRate - SourcePenetrationRate;

	Damage = (1 - EffectiveArmorRate) * Damage;
	
	/*
	 * Critical
	 */
	float SourceCriticalChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalChanceDef, EvaluationParameters, SourceCriticalChance);
	SourceCriticalChance = FMath::Max<float>(SourceCriticalChance, 0.f);

	float SourceCriticalDamage = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalDamageDef, EvaluationParameters, SourceCriticalDamage);
	SourceCriticalDamage = FMath::Max<float>(SourceCriticalDamage, 0.f);

	const bool bCriticalHit = FMath::RandRange(1, 100) < SourceCriticalChance;

	UCubeAbilitySystemLibrary::SetIsCriticalHit(EffectContextHandle, bCriticalHit);

	Damage = bCriticalHit ? Damage * SourceCriticalDamage : Damage;

	const FGameplayModifierEvaluatedData EvaluatedData(UCubeAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Additive, Damage);
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
}
