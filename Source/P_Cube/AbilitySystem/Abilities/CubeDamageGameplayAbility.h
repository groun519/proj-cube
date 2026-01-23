// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "P_Cube/CubeAbilityTypes.h"
#include "P_Cube/AbilitySystem/Abilities/CubeGameplayAbility.h"
#include "P_Cube/Interaction/CombatInterface.h"

#include "P_Cube/GAS/Data/QVDamageTypes.h"

#include "CubeDamageGameplayAbility.generated.h"

UENUM(BlueprintType)
enum class ERunes : uint8
{
	Rune_Specialization,
	Rune_Size_Increase,
	Rune_Count_Increase,
	Rune_Repeat,
	Rune_Damage_Increase,
	Rune_Speed_Increase,
	Rune_Duration_Increase,
	None
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

	UFUNCTION(BlueprintPure)
	FDamageEffectParams MakeDamageEffectParamsFromClassDefaults(AActor* TargetActor = nullptr, FName NameIndex = "") const;
	
	bool ApplyCrowdControll(FDamageEffectParams& DEP, AActor* TargetActor, AActor* AvatarActor) const;
protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	TMap<FName, FDamageInfo> DamageInfoMap;

	UFUNCTION(BlueprintPure)
	FTaggedMontage GetRandomTaggedMontageFromArray(const TArray<FTaggedMontage>& TaggedMontages) const;	

	//float GetDamageByDamageType(float InLevel, const FGameplayTag& DamageType); -> 없앰.
};
