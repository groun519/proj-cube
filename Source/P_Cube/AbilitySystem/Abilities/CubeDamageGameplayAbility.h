// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "P_Cube/CubeAbilityTypes.h"
#include "P_Cube/AbilitySystem/Abilities/CubeGameplayAbility.h"
#include "P_Cube/Interaction/CombatInterface.h"
#include "CubeDamageGameplayAbility.generated.h"

USTRUCT(BlueprintType)
struct FDamageCoeff // 계수를 다루는 구조체
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag Attribute;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FScalableFloat Coeff;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bIsTarget;
};


USTRUCT(BlueprintType)
struct FDamageInfo // 피해 정보 구조체
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag Type;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FScalableFloat BaseDamage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FDamageCoeff> Coeffs; // 계수 배열 (여러 계수를 한 피해에 넣을 수 있게 하기 위함.)
};

/**
 * 
 */
UCLASS()
class P_CUBE_API UCubeDamageGameplayAbility : public UCubeGameplayAbility
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintCallable)
	void CauseDamage(AActor* TargetActor, FName Index);

	FDamageEffectParams MakeDamageEffectParamsFromClassDefaults(AActor* TargetActor = nullptr, FName NameIndex = "") const;
protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	TMap<FName, FDamageInfo> DamageInfoMap;

	UFUNCTION(BlueprintPure)
	FTaggedMontage GetRandomTaggedMontageFromArray(const TArray<FTaggedMontage>& TaggedMontages) const;

	//float GetDamageByDamageType(float InLevel, const FGameplayTag& DamageType); -> 없앰.
};
