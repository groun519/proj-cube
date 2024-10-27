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
	FGameplayEffectAttributeCaptureDefinition TargetPhysicalPowerDef;
	FGameplayEffectAttributeCaptureDefinition SourcePhysicalPowerDef;

	FGameplayEffectAttributeCaptureDefinition TargetMagicalPowerDef;
	FGameplayEffectAttributeCaptureDefinition SourceMagicalPowerDef;

	FGameplayEffectAttributeCaptureDefinition TargetHealthDef;
	FGameplayEffectAttributeCaptureDefinition SourceHealthDef;

	FGameplayEffectAttributeCaptureDefinition TargetMaxHealthDef;
	FGameplayEffectAttributeCaptureDefinition SourceMaxHealthDef;

	FGameplayEffectAttributeCaptureDefinition TargetManaDef;
	FGameplayEffectAttributeCaptureDefinition SourceManaDef;

	FGameplayEffectAttributeCaptureDefinition TargetMaxManaDef;
	FGameplayEffectAttributeCaptureDefinition SourceMaxManaDef;

	FGameplayEffectAttributeCaptureDefinition TargetMovementSpeedDef;
	FGameplayEffectAttributeCaptureDefinition SourceMovementSpeedDef;

	FGameplayEffectAttributeCaptureDefinition TargetAttackSpeedDef;
	FGameplayEffectAttributeCaptureDefinition SourceAttackSpeedDef;


	//DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);
	FGameplayEffectAttributeCaptureDefinition TargetArmorDef;
	FGameplayEffectAttributeCaptureDefinition SourceArmorDef;

	//DECLARE_ATTRIBUTE_CAPTUREDEF(ArmorPenetration);
	FGameplayEffectAttributeCaptureDefinition TargetArmorPenetrationDef;
	FGameplayEffectAttributeCaptureDefinition SourceArmorPenetrationDef;

	DECLARE_ATTRIBUTE_CAPTUREDEF(ArmorPenetrationRate);

	//DECLARE_ATTRIBUTE_CAPTUREDEF(MagicResistance);
	FGameplayEffectAttributeCaptureDefinition TargetMagicResistanceDef;
	FGameplayEffectAttributeCaptureDefinition SourceMagicResistanceDef;

	//DECLARE_ATTRIBUTE_CAPTUREDEF(MagicResistancePenetration);
	FGameplayEffectAttributeCaptureDefinition TargetMagicResistancePenetrationDef;
	FGameplayEffectAttributeCaptureDefinition SourceMagicResistancePenetrationDef;

	DECLARE_ATTRIBUTE_CAPTUREDEF(MagicResistancePenetrationRate);

	//DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalChance);
	FGameplayEffectAttributeCaptureDefinition TargetCriticalChanceDef;
	FGameplayEffectAttributeCaptureDefinition SourceCriticalChanceDef;

	//DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalDamage);
	FGameplayEffectAttributeCaptureDefinition TargetCriticalDamageDef;
	FGameplayEffectAttributeCaptureDefinition SourceCriticalDamageDef;

	CubeDamageStatics()
	{
		SourcePhysicalPowerDef =
			FGameplayEffectAttributeCaptureDefinition(UCubeAttributeSet::GetPhysicalPowerAttribute(),
				EGameplayEffectAttributeCaptureSource::Source, false);
		TargetPhysicalPowerDef =
			FGameplayEffectAttributeCaptureDefinition(UCubeAttributeSet::GetPhysicalPowerAttribute(),
				EGameplayEffectAttributeCaptureSource::Target, false);

		SourceMagicalPowerDef =
			FGameplayEffectAttributeCaptureDefinition(UCubeAttributeSet::GetMagicalPowerAttribute(),
				EGameplayEffectAttributeCaptureSource::Source, false);
		TargetMagicalPowerDef =
			FGameplayEffectAttributeCaptureDefinition(UCubeAttributeSet::GetMagicalPowerAttribute(),
				EGameplayEffectAttributeCaptureSource::Target, false);

		SourceHealthDef =
			FGameplayEffectAttributeCaptureDefinition(UCubeAttributeSet::GetHealthAttribute(),
				EGameplayEffectAttributeCaptureSource::Source, false);
		TargetHealthDef =
			FGameplayEffectAttributeCaptureDefinition(UCubeAttributeSet::GetHealthAttribute(),
				EGameplayEffectAttributeCaptureSource::Target, false);

		SourceMaxHealthDef =
			FGameplayEffectAttributeCaptureDefinition(UCubeAttributeSet::GetMaxHealthAttribute(),
				EGameplayEffectAttributeCaptureSource::Source, false);
		TargetMaxHealthDef =
			FGameplayEffectAttributeCaptureDefinition(UCubeAttributeSet::GetMaxHealthAttribute(),
				EGameplayEffectAttributeCaptureSource::Target, false);

		SourceManaDef =
			FGameplayEffectAttributeCaptureDefinition(UCubeAttributeSet::GetManaAttribute(),
				EGameplayEffectAttributeCaptureSource::Source, false);
		TargetManaDef =
			FGameplayEffectAttributeCaptureDefinition(UCubeAttributeSet::GetManaAttribute(),
				EGameplayEffectAttributeCaptureSource::Target, false);

		SourceMaxManaDef =
			FGameplayEffectAttributeCaptureDefinition(UCubeAttributeSet::GetMaxManaAttribute(),
				EGameplayEffectAttributeCaptureSource::Source, false);
		TargetMaxManaDef =
			FGameplayEffectAttributeCaptureDefinition(UCubeAttributeSet::GetMaxManaAttribute(),
				EGameplayEffectAttributeCaptureSource::Target, false);

		SourceMovementSpeedDef =
			FGameplayEffectAttributeCaptureDefinition(UCubeAttributeSet::GetMovementSpeedAttribute(),
				EGameplayEffectAttributeCaptureSource::Source, false);
		TargetMovementSpeedDef =
			FGameplayEffectAttributeCaptureDefinition(UCubeAttributeSet::GetMovementSpeedAttribute(),
				EGameplayEffectAttributeCaptureSource::Target, false);

		SourceAttackSpeedDef =
			FGameplayEffectAttributeCaptureDefinition(UCubeAttributeSet::GetAttackSpeedAttribute(),
				EGameplayEffectAttributeCaptureSource::Source, false);
		TargetAttackSpeedDef =
			FGameplayEffectAttributeCaptureDefinition(UCubeAttributeSet::GetAttackSpeedAttribute(),
				EGameplayEffectAttributeCaptureSource::Target, false);


		//DEFINE_ATTRIBUTE_CAPTUREDEF(UCubeAttributeSet, Armor, Target, false);
		SourceArmorDef =
			FGameplayEffectAttributeCaptureDefinition(UCubeAttributeSet::GetArmorAttribute(),
				EGameplayEffectAttributeCaptureSource::Source, false);
		TargetArmorDef =
			FGameplayEffectAttributeCaptureDefinition(UCubeAttributeSet::GetArmorAttribute(),
				EGameplayEffectAttributeCaptureSource::Target, false);

		//DEFINE_ATTRIBUTE_CAPTUREDEF(UCubeAttributeSet, ArmorPenetration, Source, false);
		SourceArmorPenetrationDef =
			FGameplayEffectAttributeCaptureDefinition(UCubeAttributeSet::GetArmorPenetrationAttribute(),
				EGameplayEffectAttributeCaptureSource::Source, false);
		TargetArmorPenetrationDef =
			FGameplayEffectAttributeCaptureDefinition(UCubeAttributeSet::GetArmorPenetrationAttribute(),
				EGameplayEffectAttributeCaptureSource::Target, false);

		DEFINE_ATTRIBUTE_CAPTUREDEF(UCubeAttributeSet, ArmorPenetrationRate, Source, false);

		//DEFINE_ATTRIBUTE_CAPTUREDEF(UCubeAttributeSet, MagicResistance, Target, false);
		SourceMagicResistanceDef =
			FGameplayEffectAttributeCaptureDefinition(UCubeAttributeSet::GetMagicResistanceAttribute(),
				EGameplayEffectAttributeCaptureSource::Source, false);
		TargetMagicResistanceDef =
			FGameplayEffectAttributeCaptureDefinition(UCubeAttributeSet::GetMagicResistanceAttribute(),
				EGameplayEffectAttributeCaptureSource::Target, false);

		//DEFINE_ATTRIBUTE_CAPTUREDEF(UCubeAttributeSet, MagicResistancePenetration, Source, false);
		SourceMagicResistancePenetrationDef =
			FGameplayEffectAttributeCaptureDefinition(UCubeAttributeSet::GetMagicResistancePenetrationAttribute(),
				EGameplayEffectAttributeCaptureSource::Source, false);
		TargetMagicResistancePenetrationDef =
			FGameplayEffectAttributeCaptureDefinition(UCubeAttributeSet::GetMagicResistancePenetrationAttribute(),
				EGameplayEffectAttributeCaptureSource::Target, false);

		DEFINE_ATTRIBUTE_CAPTUREDEF(UCubeAttributeSet, MagicResistancePenetrationRate, Source, false);

		//DEFINE_ATTRIBUTE_CAPTUREDEF(UCubeAttributeSet, CriticalChance, Source, false);
		SourceCriticalChanceDef =
			FGameplayEffectAttributeCaptureDefinition(UCubeAttributeSet::GetCriticalChanceAttribute(),
				EGameplayEffectAttributeCaptureSource::Source, false);
		TargetCriticalChanceDef =
			FGameplayEffectAttributeCaptureDefinition(UCubeAttributeSet::GetCriticalChanceAttribute(),
				EGameplayEffectAttributeCaptureSource::Target, false);

		//DEFINE_ATTRIBUTE_CAPTUREDEF(UCubeAttributeSet, CriticalDamage, Source, false);
		SourceCriticalChanceDef =
			FGameplayEffectAttributeCaptureDefinition(UCubeAttributeSet::GetCriticalDamageAttribute(),
				EGameplayEffectAttributeCaptureSource::Source, false);
		TargetCriticalChanceDef =
			FGameplayEffectAttributeCaptureDefinition(UCubeAttributeSet::GetCriticalDamageAttribute(),
				EGameplayEffectAttributeCaptureSource::Target, false);
	}
};

static const CubeDamageStatics& DamageStatics()
{
	static CubeDamageStatics DStatics;
	return DStatics;
}

UExecCalc_Damage::UExecCalc_Damage()
{
	RelevantAttributesToCapture.Add(DamageStatics().SourcePhysicalPowerDef);
	RelevantAttributesToCapture.Add(DamageStatics().SourceMagicalPowerDef);
	RelevantAttributesToCapture.Add(DamageStatics().SourceHealthDef);
	RelevantAttributesToCapture.Add(DamageStatics().SourceMaxHealthDef);
	RelevantAttributesToCapture.Add(DamageStatics().SourceManaDef);
	RelevantAttributesToCapture.Add(DamageStatics().SourceMaxManaDef);
	RelevantAttributesToCapture.Add(DamageStatics().SourceMovementSpeedDef);
	RelevantAttributesToCapture.Add(DamageStatics().SourceAttackSpeedDef);
	RelevantAttributesToCapture.Add(DamageStatics().SourceArmorDef);
	RelevantAttributesToCapture.Add(DamageStatics().SourceArmorPenetrationDef);
	RelevantAttributesToCapture.Add(DamageStatics().SourceMagicResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().SourceMagicResistancePenetrationDef);
	RelevantAttributesToCapture.Add(DamageStatics().SourceCriticalChanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().SourceCriticalDamageDef);

	RelevantAttributesToCapture.Add(DamageStatics().TargetPhysicalPowerDef);
	RelevantAttributesToCapture.Add(DamageStatics().TargetMagicalPowerDef);
	RelevantAttributesToCapture.Add(DamageStatics().TargetHealthDef);
	RelevantAttributesToCapture.Add(DamageStatics().TargetMaxHealthDef);
	RelevantAttributesToCapture.Add(DamageStatics().TargetManaDef);
	RelevantAttributesToCapture.Add(DamageStatics().TargetMaxManaDef);
	RelevantAttributesToCapture.Add(DamageStatics().TargetMovementSpeedDef);
	RelevantAttributesToCapture.Add(DamageStatics().TargetAttackSpeedDef);
	RelevantAttributesToCapture.Add(DamageStatics().TargetArmorDef);
	RelevantAttributesToCapture.Add(DamageStatics().TargetArmorPenetrationDef);
	RelevantAttributesToCapture.Add(DamageStatics().TargetMagicResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().TargetMagicResistancePenetrationDef);
	RelevantAttributesToCapture.Add(DamageStatics().TargetCriticalChanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().TargetCriticalDamageDef);

	RelevantAttributesToCapture.Add(DamageStatics().ArmorPenetrationRateDef);
	RelevantAttributesToCapture.Add(DamageStatics().MagicResistancePenetrationRateDef);
}

void UExecCalc_Damage::DetermineDebuff(const FGameplayEffectCustomExecutionParameters& ExecutionParams, const FGameplayEffectSpec& Spec, 
	FAggregatorEvaluateParameters EvaluationParameters, const FGameplayTag& DamageType, const FGameplayTag& DebuffType,
	const TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition>& InTagsToDefs) const
{
	const FCubeGameplayTags& GameplayTags = FCubeGameplayTags::Get();

	const float TypeDamage = Spec.GetSetByCallerMagnitude(DamageType, false, -1.f);
	if ( TypeDamage > -.5f ) // .5 padding for floating point [im]precision
	{
		// Determine if there was a successful debuff
		float TargetResistance = 0.f;
		float TargetResistanceRate = 0.f;
		float SourcePenetration = 0.f;
		float SourcePenetrationRate = 0.f;


		const FGameplayTag& ResistanceTag = GameplayTags.DamageTypesToResistanceInfos[ DamageType ].Resistance;
		const FGameplayTag& ResistanceRateTag = GameplayTags.DamageTypesToResistanceInfos[ DamageType ].ResistanceRate;
		const FGameplayTag& PenetrationTag = GameplayTags.DamageTypesToResistanceInfos[ DamageType ].Penetration;
		const FGameplayTag& PenetrationRateTag = GameplayTags.DamageTypesToResistanceInfos[ DamageType ].PenetrationRate;
		

		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(InTagsToDefs[ ResistanceTag ], EvaluationParameters, TargetResistance);
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(InTagsToDefs[ ResistanceRateTag ], EvaluationParameters, TargetResistanceRate);
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(InTagsToDefs[ PenetrationTag ], EvaluationParameters, SourcePenetration);
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(InTagsToDefs[ PenetrationRateTag ], EvaluationParameters, SourcePenetrationRate);
		
		TargetResistance = FMath::Max<float>(TargetResistance, 0.f);
		TargetResistanceRate = FMath::Max<float>(TargetResistanceRate, 0.f);
		SourcePenetration = FMath::Max<float>(SourcePenetration, 0.f);
		SourcePenetrationRate = FMath::Max<float>(SourcePenetrationRate, 0.f);
	}
}

void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition> TagsToCaptureDefs;
	const FCubeGameplayTags& Tags = FCubeGameplayTags::Get();

	/*TagsToCaptureDefs.Add(Tags.Attributes_Primary_Armor, DamageStatics().ArmorDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Primary_ArmorPenetration, DamageStatics().ArmorPenetrationDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Primary_ArmorPenetrationRate, DamageStatics().ArmorPenetrationRateDef);

	TagsToCaptureDefs.Add(Tags.Attributes_Primary_MagicResistance, DamageStatics().MagicResistanceDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Primary_MagicResistancePenetration, DamageStatics().MagicResistancePenetrationDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Primary_MagicResistancePenetrationRate, DamageStatics().MagicResistancePenetrationRateDef);

	TagsToCaptureDefs.Add(Tags.Attributes_Primary_CriticalChance, DamageStatics().CriticalChanceDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Primary_CriticalDamage, DamageStatics().CriticalDamageDef);*/


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

	// Debuff <- 음...
	//DetermineDebuff(ExecutionParams, Spec, EvaluationParameters, FCubeGameplayTags::Get().Damage_Magical, FCubeGameplayTags::Get().Debuff_Burn, TagsToCaptureDefs);

	FGameplayEffectContextHandle EffectContextHandle = Spec.GetContext();

	// Set by Caller로 설정된 피해 값을 가져옴
	float Damage = 0.f;

	for ( FGameplayTag CoeffAttTag : FCubeGameplayTags::Get().DamageCoeffAttributes )
	{
		const float DamageCoeffWithSign = Spec.GetSetByCallerMagnitude(CoeffAttTag, false);

		if ( DamageCoeffWithSign == 0 ) continue;

		float DamageCoeffAttribute = 0.f;

		if ( DamageCoeffWithSign < 0 ) // Target
		{
			if ( CoeffAttTag.GetTagName() == TEXT("Attributes.Primary.PhysicalPower") )
				ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().TargetPhysicalPowerDef, EvaluationParameters, DamageCoeffAttribute);

			else if ( CoeffAttTag.GetTagName() == TEXT("Attributes.Primary.MagicalPower") )
				ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().TargetMagicalPowerDef, EvaluationParameters, DamageCoeffAttribute);

			else if ( CoeffAttTag.GetTagName() == TEXT("Attributes.Vital.Health") )
				ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().TargetHealthDef, EvaluationParameters, DamageCoeffAttribute);

			else if ( CoeffAttTag.GetTagName() == TEXT("Attributes.Secondary.MaxHealth") )
				ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().TargetMaxHealthDef, EvaluationParameters, DamageCoeffAttribute);

			else if ( CoeffAttTag.GetTagName() == TEXT("Attributes.Vital.Mana") )
				ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().TargetManaDef, EvaluationParameters, DamageCoeffAttribute);

			else if ( CoeffAttTag.GetTagName() == TEXT("Attributes.Secondary.MaxMana") )
				ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().TargetMaxManaDef, EvaluationParameters, DamageCoeffAttribute);

			else if ( CoeffAttTag.GetTagName() == TEXT("Attributes.Primary.MovementSpeed") )
				ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().TargetMovementSpeedDef, EvaluationParameters, DamageCoeffAttribute);

			else if ( CoeffAttTag.GetTagName() == TEXT("Attributes.Primary.AttackSpeed") )
				ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().TargetAttackSpeedDef, EvaluationParameters, DamageCoeffAttribute);

			else if ( CoeffAttTag.GetTagName() == TEXT("Attributes.Primary.Armor") )
				ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().TargetArmorDef, EvaluationParameters, DamageCoeffAttribute);

			else if ( CoeffAttTag.GetTagName() == TEXT("Attributes.Primary.ArmorPenetration") )
				ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().TargetArmorPenetrationDef, EvaluationParameters, DamageCoeffAttribute);

			else if ( CoeffAttTag.GetTagName() == TEXT("Attributes.Primary.MagicResistance") )
				ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().TargetMagicResistanceDef, EvaluationParameters, DamageCoeffAttribute);

			else if ( CoeffAttTag.GetTagName() == TEXT("Attributes.Primary.MagicResistancePenetration") )
				ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().TargetMagicResistancePenetrationDef, EvaluationParameters, DamageCoeffAttribute);

			else if ( CoeffAttTag.GetTagName() == TEXT("Attributes.Primary.CriticalChance") )
				ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().TargetCriticalChanceDef, EvaluationParameters, DamageCoeffAttribute);

			else if ( CoeffAttTag.GetTagName() == TEXT("Attributes.Primary.CriticalDamage") )
				ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().TargetCriticalDamageDef, EvaluationParameters, DamageCoeffAttribute);

			DamageCoeffAttribute = DamageCoeffAttribute * -DamageCoeffWithSign;
		}
		else
		{
			if ( CoeffAttTag.GetTagName() == TEXT("Attributes.Primary.PhysicalPower") )
				ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().SourcePhysicalPowerDef, EvaluationParameters, DamageCoeffAttribute);

			else if ( CoeffAttTag.GetTagName() == TEXT("Attributes.Primary.MagicalPower") )
				ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().SourceMagicalPowerDef, EvaluationParameters, DamageCoeffAttribute);

			else if ( CoeffAttTag.GetTagName() == TEXT("Attributes.Vital.Health") )
				ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().SourceHealthDef, EvaluationParameters, DamageCoeffAttribute);

			else if ( CoeffAttTag.GetTagName() == TEXT("Attributes.Secondary.MaxHealth") )
				ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().SourceMaxHealthDef, EvaluationParameters, DamageCoeffAttribute);

			else if ( CoeffAttTag.GetTagName() == TEXT("Attributes.Vital.Mana") )
				ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().SourceManaDef, EvaluationParameters, DamageCoeffAttribute);

			else if ( CoeffAttTag.GetTagName() == TEXT("Attributes.Secondary.MaxMana") )
				ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().SourceMaxManaDef, EvaluationParameters, DamageCoeffAttribute);

			else if ( CoeffAttTag.GetTagName() == TEXT("Attributes.Primary.MovementSpeed") )
				ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().SourceMovementSpeedDef, EvaluationParameters, DamageCoeffAttribute);

			else if ( CoeffAttTag.GetTagName() == TEXT("Attributes.Primary.AttackSpeed") )
				ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().SourceAttackSpeedDef, EvaluationParameters, DamageCoeffAttribute);

			else if ( CoeffAttTag.GetTagName() == TEXT("Attributes.Primary.Armor") )
				ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().SourceArmorDef, EvaluationParameters, DamageCoeffAttribute);

			else if ( CoeffAttTag.GetTagName() == TEXT("Attributes.Primary.ArmorPenetration") )
				ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().SourceArmorPenetrationDef, EvaluationParameters, DamageCoeffAttribute);

			else if ( CoeffAttTag.GetTagName() == TEXT("Attributes.Primary.MagicResistance") )
				ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().SourceMagicResistanceDef, EvaluationParameters, DamageCoeffAttribute);

			else if ( CoeffAttTag.GetTagName() == TEXT("Attributes.Primary.MagicResistancePenetration") )
				ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().SourceMagicResistancePenetrationDef, EvaluationParameters, DamageCoeffAttribute);

			else if ( CoeffAttTag.GetTagName() == TEXT("Attributes.Primary.CriticalChance") )
				ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().SourceCriticalChanceDef, EvaluationParameters, DamageCoeffAttribute);

			else if ( CoeffAttTag.GetTagName() == TEXT("Attributes.Primary.CriticalDamage") )
				ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().SourceCriticalDamageDef, EvaluationParameters, DamageCoeffAttribute);

			DamageCoeffAttribute = DamageCoeffAttribute * DamageCoeffWithSign; // 계수 비례 피해량 계산
		}
		
		DamageCoeffAttribute = FMath::Max<float>(DamageCoeffAttribute, 0.f); // 음수 피해량 따위는 허용하지 않는다.
		Damage = Damage + DamageCoeffAttribute; // 계수 만큼의 피해량 합산
	}


	float SourcePenetration = 0.f;
	float SourcePenetrationRate = 0.f;
	int32 DamageType = 0;

	for (FGameplayTag DamageTypeTag : FCubeGameplayTags::Get().DamageTypes)
	{
		float DamageTypeValue = Spec.GetSetByCallerMagnitude(DamageTypeTag, false);

		if ( DamageTypeValue == 0 ) continue; // 만약 할당 안됐으면 countinue
		if ( DamageTypeValue < 0 ) DamageTypeValue = 0; // 할당은 됐는데 BaseDamage를 안 쓰고 싶다면 음수값으로 두면 됨. 그러면 0으로 처리.

		if (DamageTypeTag.GetTagName() == TEXT("Damage.Physical"))
		{
			ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().SourceArmorPenetrationDef, EvaluationParameters, SourcePenetration);
			SourcePenetration = FMath::Max<float>(SourcePenetration, 0.f);

			ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorPenetrationRateDef, EvaluationParameters, SourcePenetrationRate);
			SourcePenetrationRate = FMath::Max<float>(SourcePenetrationRate, 0.f);

			DamageType = 0;
			UCubeAbilitySystemLibrary::SetIsPhysicalHit(EffectContextHandle, true);
		}
		else if (DamageTypeTag.GetTagName() == TEXT("Damage.Magical"))
		{
			ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().SourceMagicResistancePenetrationDef, EvaluationParameters, SourcePenetration);
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
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().TargetArmorDef, EvaluationParameters, TargetResistance);
		TargetResistance = FMath::Max<float>(TargetResistance, 0.f);

		// Armor 값을 기반으로 ArmorRate 값을 커브 테이블에서 가져옴
		const UCharacterClassInfo* CharacterClassInfo = UCubeAbilitySystemLibrary::GetCharacterClassInfo(SourceAvatar);
		const FRealCurve* ResistanceRateCurve = CharacterClassInfo->ResistanceRateCurveTable->FindCurve(FName("ArmorRate"), FString());
		TargetResistanceRate = ResistanceRateCurve->Eval(TargetResistance - SourcePenetration);
	}
	else if (DamageType == 1)
	{
		// 타겟의 MagicResistance 값을 캡처하여 가져옴
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().TargetMagicResistanceDef, EvaluationParameters, TargetResistance);
		TargetResistance = FMath::Max<float>(TargetResistance, 0.f);

		// MagicResistance 값을 기반으로 MagicResistanceRate 값을 커브 테이블에서 가져옴
		const UCharacterClassInfo* CharacterClassInfo = UCubeAbilitySystemLibrary::GetCharacterClassInfo(SourceAvatar);
		const FRealCurve* ResistanceRateCurve = CharacterClassInfo->ResistanceRateCurveTable->FindCurve(FName("MagicResistanceRate"), FString());
		TargetResistanceRate = ResistanceRateCurve->Eval(TargetResistance - SourcePenetration);
	}

	const float EffectiveArmorRate = TargetResistanceRate - SourcePenetrationRate;

	Damage = (1 - EffectiveArmorRate) * Damage;
	
	/*
	 * Critical
	 */
	float SourceCriticalChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().SourceCriticalChanceDef, EvaluationParameters, SourceCriticalChance);
	SourceCriticalChance = FMath::Max<float>(SourceCriticalChance, 0.f);

	float SourceCriticalDamage = 0.f;
	if ( DamageType == 0 ) // PhysicalDamage만 치명타 적용.
	{
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().SourceCriticalDamageDef, EvaluationParameters, SourceCriticalDamage);
		SourceCriticalDamage = FMath::Max<float>(SourceCriticalDamage, 0.f);
	}

	const bool bCriticalHit = FMath::RandRange(1, 100) < SourceCriticalChance;

	UCubeAbilitySystemLibrary::SetIsCriticalHit(EffectContextHandle, bCriticalHit);

	Damage = bCriticalHit ? Damage * SourceCriticalDamage : Damage;

	const FGameplayModifierEvaluatedData EvaluatedData(UCubeAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Additive, Damage);
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
}
