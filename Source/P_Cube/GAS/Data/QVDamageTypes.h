#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "ScalableFloat.h"
#include "QVDamageTypes.generated.h"

class UGameplayEffect;

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
enum class EBindType : uint8
{
	None UMETA(DisplayName = "None"),
	Knockback UMETA(DisplayName = "Knockback"),
	Grab UMETA(DisplayName = "Grab"),
	Airborne UMETA(DisplayName = "Airborne"),
	Stun UMETA(DisplayName = "Stun"),
	Slow UMETA(DisplayName = "Slow"),
	/*Root UMETA(DisplayName = "Root"),
	Silence UMETA(DisplayName = "Silence")*/
};

USTRUCT(BlueprintType)
struct FCCEffect
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EBindType Type = EBindType::None;

	/** Knockback **/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Knockback", meta = ( EditCondition = "Type == EBindType::Knockback", EditConditionHides ))
	float Knockback_ForceMagnitude = 0.f;
	/** end Knockback **/

	/** Grab **/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grab", meta = ( EditCondition = "Type == EBindType::Grab", EditConditionHides ))
	float Grab_ForceMagnitude = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grab", meta = ( EditCondition = "Type == EBindType::Grab", EditConditionHides ))
	FVector Grab_Offset = FVector::ZeroVector; // 내 위치에서 어디로 그랩되게 할지
	/** end Grab **/

	/** Airborne **/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Airborne", meta = ( EditCondition = "Type == EBindType::Airborne", EditConditionHides ))
	float Airborne_ForceMagnitude = 0.f;
	/** end Airborne **/

	/** Stun **/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stun", meta = ( EditCondition = "Type == EBindType::Stun", EditConditionHides ))
	float Stun_Time;
	/** end Stun **/

	/** Slow **/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slow", meta = ( EditCondition = "Type == EBindType::Slow", EditConditionHides ))
	float Slow_Time;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slow", meta = ( EditCondition = "Type == EBindType::Slow", EditConditionHides ))
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
	bool bBlockCritical = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FDamageCoeff> Coeffs; // 계수 배열 (여러 계수를 한 피해에 넣을 수 있게 하기 위함.)

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FCCEffect> CrowdControlEffects;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<TSubclassOf<UGameplayEffect>> Effects;
};
