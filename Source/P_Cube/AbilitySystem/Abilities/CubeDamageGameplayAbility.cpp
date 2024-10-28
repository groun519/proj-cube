// Fill out your copyright notice in the Description page of Project Settings.


#include "CubeDamageGameplayAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

void UCubeDamageGameplayAbility::CauseDamage(AActor* TargetActor, FName Index) // TargetActor에게 Index번째의 피해정보를 토대로 피해를 적용함.
{
	FGameplayEffectSpecHandle DamageSpecHandle = MakeOutgoingGameplayEffectSpec(DamageEffectClass, 1.f);
	for (TTuple<FName, FDamageInfo> Pair : DamageInfoMap )
	{
		TArray<FDamageCoeff> DamageCoeffArr = Pair.Value.Coeffs;
		for ( FDamageCoeff DamageCoeff : DamageCoeffArr )
		{
			const FGameplayTag Att = DamageCoeff.Attribute;
			float Coeff = DamageCoeff.Coeff.GetValueAtLevel(GetAbilityLevel());

			if ( DamageCoeff.bIsTarget ) // Target
			{
				UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpecHandle, Att, -Coeff);
			}
			else // Source
			{
				UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpecHandle, Att, Coeff);
			}
		}
		const FGameplayTag DamageType = Pair.Value.Type;
		const float BaseDamage = Pair.Value.BaseDamage.GetValueAtLevel(GetAbilityLevel());
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpecHandle, DamageType, BaseDamage);
			//	DamageType.GetTagName() == TEXT("Damage.Physical")	?	0  // 0 -> 물리피해
			//:	DamageType.GetTagName() == TEXT("Damage.Magical")	?	1  // 1 -> 마법피해
			//:	DamageType.GetTagName() == TEXT("Damage.Pure")		?	2  // 2 -> 순수피해 (고정피해)
			//:															3);// 3 -> 에러값. TODO : 만약 3이 들어온다면, check()를 통해 오류 문구를 출력할 것.
	}
	GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(*DamageSpecHandle.Data.Get(), UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor));
}

FDamageEffectParams UCubeDamageGameplayAbility::MakeDamageEffectParamsFromClassDefaults(AActor* TargetActor, FName NameIndex) const
{
	FDamageEffectParams Params;
	Params.WorldContextObject = GetAvatarActorFromActorInfo();
	Params.DamageGameplayEffectClass = DamageEffectClass;
	Params.SourceAbilitySystemComponent = GetAbilitySystemComponentFromActorInfo();
	Params.TargetAbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);


	if ( DamageInfoMap.Contains(NameIndex) )
	{
		const FDamageInfo& DamageInfo = DamageInfoMap[ NameIndex ];

		Params.BaseDamage = DamageInfo.BaseDamage.GetValueAtLevel(GetAbilityLevel());
		Params.AbilityLevel = GetAbilityLevel();
		Params.DamageType = DamageInfo.Type;
		/*Params.Duration = DamageInfo.Duration;
		Params.Frequency = DamageInfo.Frequency;*/
		
		/*if ( DamageInfo.bHasDuration )
		{
			UGameplayEffect* Effect = NewObject<UGameplayEffect>(GetTransientPackage());
			Effect->DurationPolicy = EGameplayEffectDurationType::HasDuration;
			Effect->Period = DamageInfo.Frequency;
			Effect->DurationMagnitude = FScalableFloat(DamageInfo.Duration);
			Effect->StackingType = EGameplayEffectStackingType::AggregateBySource;
			Effect->StackLimitCount = 1;

			Params.DamageGameplayEffectClass = Effect->GetClass();
		}*/

		// Coeffs 배열이 비어있는지 확인
		if ( DamageInfo.Coeffs.Num() > 0 )
		{
			for ( FDamageCoeff DamageCoeff : DamageInfo.Coeffs )
			{
				FCoeffs AttCoeff;
				AttCoeff.Attribute = DamageCoeff.Attribute;
				AttCoeff.Coeff = DamageCoeff.Coeff.GetValueAtLevel(GetAbilityLevel());
				AttCoeff.bIsTarget = DamageCoeff.bIsTarget;

				Params.AttributeCoeffs.Add(AttCoeff);
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("DamageInfoMap does not contain NameIndex: %s"), *NameIndex.ToString());
	}

	return Params;
}

FTaggedMontage UCubeDamageGameplayAbility::GetRandomTaggedMontageFromArray(const TArray<FTaggedMontage>& TaggedMontages) const
{
	if (TaggedMontages.Num() > 0)
	{
		const int32 Selection = FMath::RandRange(0, TaggedMontages.Num() - 1);
		return TaggedMontages[Selection];
	}

	return FTaggedMontage();
}

//float UCubeDamageGameplayAbility::GetDamageByDamageType(float InLevel, const FGameplayTag& DamageType)
//{
//	checkf(DamageTypes.Contains(DamageType), TEXT("GameplayAbilit [%s] does not contain DamageType [%s]"), *GetNameSafe(this), *DamageType.ToString());
//	return DamageTypes[DamageType].GetValueAtLevel(InLevel);
//}
