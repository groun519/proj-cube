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

UENUM(BlueprintType)
enum class ECCType : uint8
{
	None UMETA(DisplayName = "None"),
	Knockback UMETA(DisplayName = "Knockback"),
	Grab UMETA(DisplayName = "Grab"),
	Airborne UMETA(DisplayName = "Airborne"),
	Stun UMETA(DisplayName = "Stun"),
	Slow UMETA(DisplayName = "Slow")
};

USTRUCT(BlueprintType)
struct FCCEffect
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ECCType Type = ECCType::None;


	/** Knockback **/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Knockback", meta = ( EditCondition = "Type == ECCType::Knockback", EditConditionHides ))
	float Knockback_ForceMagnitude = 0.f;
	/** end Knockback **/

	/** Grab **/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grab", meta = ( EditCondition = "Type == ECCType::Grab", EditConditionHides ))
	float Grab_ForceMagnitude = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grab", meta = ( EditCondition = "Type == ECCType::Grab", EditConditionHides ))
	FVector Grab_Offset = FVector::ZeroVector; // 내 위치에서 어디로 그랩되게 할지
	/** end Grab **/

	/** Airborne **/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Airborne", meta = ( EditCondition = "Type == ECCType::Airborne", EditConditionHides ))
	float Airborne_ForceMagnitude = 0.f;
	/** end Airborne **/


	/** Stun **/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stun", meta = ( EditCondition = "Type == ECCType::Stun", EditConditionHides ))
	float Stun_Time;
	/** end Stun **/


	/** Slow **/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slow", meta = ( EditCondition = "Type == ECCType::Slow", EditConditionHides ))
	float Slow_Time;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slow", meta = ( EditCondition = "Type == ECCType::Slow", EditConditionHides ))
	float Slow_Rate;
	/** end Slow **/


	/** Root **/
	/** end Root **/


	/** Silence **/
	/** end Silence **/
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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FCCEffect> CrowdControlEffects;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<TSubclassOf<UGameplayEffect>> Effects;
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
