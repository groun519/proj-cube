// Fill out your copyright notice in the Description page of Project Settings.


#include "CubeDamageGameplayAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "P_Cube/CubeGameplayTags.h"

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
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpecHandle, FCubeGameplayTags::Get().BlockCritical, Pair.Value.bBlockCritical ? 1.0f : 0.0f);
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
		Params.bBlockCritical = DamageInfo.bBlockCritical;
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

		if ( DamageInfo.CrowdControlEffects.Num() > 0 )
		{
			for ( FCCEffect CCEffect : DamageInfo.CrowdControlEffects )
			{
				if ( CCEffect.Type == ECCType::Knockback )
				{
					Params.Knockback_ForceMagnitude = CCEffect.Knockback_ForceMagnitude;
					if ( IsValid(TargetActor) )
					{
						FRotator Rotation = ( TargetActor->GetActorLocation() - GetAvatarActorFromActorInfo()->GetActorLocation() ).Rotation();
						Rotation.Pitch = 0.f;
						const FVector ToTarget = Rotation.Vector();
						Params.Knockback_Force = ToTarget * CCEffect.Knockback_ForceMagnitude;
					}
				}
				else if ( CCEffect.Type == ECCType::Grab )
				{
					Params.Grab_ForceMagnitude = CCEffect.Grab_ForceMagnitude;
					if ( IsValid(TargetActor) )
					{
						FRotator Rotation = ( GetAvatarActorFromActorInfo()->GetActorLocation() - TargetActor->GetActorLocation() ).Rotation();
						Rotation.Pitch = 0.f;
						const FVector ToTarget = Rotation.Vector();
						Params.Grab_Force = ToTarget * CCEffect.Grab_ForceMagnitude;
					}
				}
				else if ( CCEffect.Type == ECCType::Airborne )
				{
					Params.Airborne_ForceMagnitude = CCEffect.Airborne_ForceMagnitude;
					if ( IsValid(TargetActor) )
					{
						FRotator Rotation = FRotator(0, 0, 1);
						Rotation.Pitch = 90.f;
						const FVector ToTarget = Rotation.Vector();
						Params.Airborne_Force = ToTarget * CCEffect.Airborne_ForceMagnitude;
					}
				}
				else if ( CCEffect.Type == ECCType::Stun )
				{
					Params.Stun_Time = CCEffect.Stun_Time;
				}
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("DamageInfoMap does not contain NameIndex: %s"), *NameIndex.ToString());
	}

	return Params;
}

void UCubeDamageGameplayAbility::SetRuneValues(FGameplayTag RuneTag, ERunes& Runes)
{
			if (RuneTag.GetTagName() == FName("Rune.Specialization"		))	{ Runes = ERunes::Rune_Specialization;		}
	else	if (RuneTag.GetTagName() == FName("Rune.Size.Increase"		))	{ Runes = ERunes::Rune_Size_Increase;		}
	else	if (RuneTag.GetTagName() == FName("Rune.Count.Increase"		))	{ Runes = ERunes::Rune_Count_Increase;		}
	else	if (RuneTag.GetTagName() == FName("Rune.Repeat"				))	{ Runes = ERunes::Rune_Repeat;				}
	else	if (RuneTag.GetTagName() == FName("Rune.Damage.Increase"	))	{ Runes = ERunes::Rune_Damage_Increase;		}
	else	if (RuneTag.GetTagName() == FName("Rune.Speed.Increase"		))	{ Runes = ERunes::Rune_Speed_Increase;		}
	else	if (RuneTag.GetTagName() == FName("Rune.Duration.Increase"	))	{ Runes = ERunes::Rune_Duration_Increase;	}
	else
			{
				Runes = ERunes::None;
			}
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

bool UCubeDamageGameplayAbility::ApplyCrowdControll(FDamageEffectParams& DEP, AActor* TargetActor, AActor* AvatarActor) const
{
	if ( AvatarActor->HasAuthority() )
	{
		if ( UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor) )
		{
			/** Knockback **/
			const bool bKnockback = DEP.Knockback_ForceMagnitude != 0.f;
			if ( bKnockback )
			{
				const FVector ActorLocation = AvatarActor->GetActorLocation();
				const FVector AttackerLocation = TargetActor->GetActorLocation();

				const FVector KnockbackDirection = ( AttackerLocation - ActorLocation ).GetSafeNormal();
				const FVector KnockbackForce = KnockbackDirection * DEP.Knockback_ForceMagnitude;
				DEP.Knockback_Force = KnockbackForce;
			}
			/** end Knockback **/

			/** Grab **/
			const bool bGrab = DEP.Grab_ForceMagnitude != 0.f;
			if ( bGrab )
			{
				const FVector ActorLocation = AvatarActor->GetActorLocation();
				const FVector AttackerLocation = TargetActor->GetActorLocation();

				const FVector GrabDirection = ( ActorLocation - AttackerLocation ).GetSafeNormal();
				const FVector GrabForce = GrabDirection * DEP.Grab_ForceMagnitude;
				DEP.Grab_Force = GrabForce;
			}
			/** end Grab **/

			/** Airborne **/
			const bool bAirborn = DEP.Airborne_ForceMagnitude != 0.f;
			if ( bAirborn )
			{
				const FVector AirborneDirection = FVector(0, 0, 1);
				const FVector AirborneForce = AirborneDirection * DEP.Airborne_ForceMagnitude;
				DEP.Airborne_Force = AirborneForce;
			}
			/** end Airborne **/

			/** Stun **/
			const bool bStun = DEP.Stun_Time != 0.f;
			if ( bStun )
			{

			}
			/** end Stun **/

			/** Slow **/

			/** end Slow **/

			DEP.TargetAbilitySystemComponent = TargetASC;
			return true;
		}
		else return false;
	}
	else return false;
}

//float UCubeDamageGameplayAbility::GetDamageByDamageType(float InLevel, const FGameplayTag& DamageType)
//{
//	checkf(DamageTypes.Contains(DamageType), TEXT("GameplayAbilit [%s] does not contain DamageType [%s]"), *GetNameSafe(this), *DamageType.ToString());
//	return DamageTypes[DamageType].GetValueAtLevel(InLevel);
//}
