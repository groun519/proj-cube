#pragma once

#include "GameplayEffectTypes.h"
#include "CubeAbilityTypes.generated.h"

class UGameplayEffect;

USTRUCT(BlueprintType)
struct FCoeffs // 계수를 다루는 구조체
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag Attribute;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Coeff;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bIsTarget;
};

USTRUCT(BlueprintType)
struct FDamageEffectParams
{
	GENERATED_BODY()

	FDamageEffectParams(){}

	UPROPERTY(BlueprintReadWrite/*BlueprintReadWrite, EditAnywhere, Category = "DamageEffectParams"*/)
	TObjectPtr<UObject> WorldContextObject = nullptr;

	UPROPERTY(BlueprintReadWrite)
	TSubclassOf<UGameplayEffect> DamageGameplayEffectClass = nullptr;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent> SourceAbilitySystemComponent;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent> TargetAbilitySystemComponent;

	


	UPROPERTY(BlueprintReadWrite)
	float AbilityLevel = 1.f;

	UPROPERTY(BlueprintReadWrite)
	FGameplayTag DamageType = FGameplayTag();

	UPROPERTY(BlueprintReadWrite)
	float BaseDamage = 0.f;

	UPROPERTY(BlueprintReadWrite)
	TArray<FCoeffs> AttributeCoeffs;


	/** Knockback **/
	UPROPERTY(BlueprintReadWrite) float Knockback_ForceMagnitude = 0.f;
	UPROPERTY(BlueprintReadWrite) FVector Knockback_Force = FVector::ZeroVector;
	/** end Knockback **/

	/** Grab **/
	UPROPERTY(BlueprintReadWrite) float Grab_ForceMagnitude = 0.f;
	UPROPERTY(BlueprintReadWrite) FVector Grab_Offset = FVector::ZeroVector;
	UPROPERTY(BlueprintReadWrite) FVector Grab_Force = FVector::ZeroVector;
	/** end Grab **/

	/** Airborne **/
	UPROPERTY(BlueprintReadWrite) float Airborne_ForceMagnitude = 0.f;
	UPROPERTY(BlueprintReadWrite) FVector Airborne_Force = FVector::ZeroVector;
	/** end Airborne **/

	/** Stun **/
	UPROPERTY(BlueprintReadWrite) float Stun_Time = 0.f;
	/** end Stun **/

	/** Slow **/
	UPROPERTY(BlueprintReadWrite) float Slow_Time = 0.f;
	UPROPERTY(BlueprintReadWrite) float Slow_Rate = 0.f;
	/** end Slow **/
};

USTRUCT(BlueprintType)
struct FCubeGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY()

public:

	/** Get **/
	bool IsCriticalHit() const { return bIsCriticalHit; }
	bool IsPhysicalHit() const { return bIsPhysicalHit; }
	bool IsMagicalHit() const { return bIsMagicalHit; }
	bool IsPureHit() const { return bIsPureHit; }
	bool IsHealHit() const { return bIsHealHit; }
	TSharedPtr<FGameplayTag> GetDamageType() const { return DamageType; }

	FVector GetKnockbackForce() const { return Knockback_Force; }
	FVector GetGrabForce() const { return Grab_Force; }
	FVector GetAirborneForce() const { return Airborne_Force; }
	float GetStunTime() const {return Stun_Time;}
	float GetSlowTime() const {return Slow_Time;}
	float GetSlowRate() const {return Slow_Rate;}

	/** Set **/
	void SetIsCriticalHit(bool bInIsCriticalHit) { bIsCriticalHit = bInIsCriticalHit; }
	void SetIsPhysicalHit(bool bInIsPhysicalHit) { bIsPhysicalHit = bInIsPhysicalHit; }
	void SetIsMagicalHit(bool bInIsMagicalHit) { bIsMagicalHit = bInIsMagicalHit; }
	void SetIsPureHit(bool bInIsPureHit) { bIsPureHit = bInIsPureHit; }
	void SetIsHealHit(bool bInIsHealHit) { bIsHealHit = bInIsHealHit; }
	void SetDamageType(TSharedPtr<FGameplayTag> InDamageType) { DamageType = InDamageType; }

	void SetKnockbackForce(const FVector& InForce) { Knockback_Force = InForce; }
	void SetGrabForce(const FVector& InForce) { Grab_Force = InForce; }
	void SetAirborneForce(const FVector& InForce) { Airborne_Force = InForce; }
	void SetStunTime(const float& InTime) { Stun_Time = InTime; }
	void SetSlowTime(const float& InTime) { Slow_Time = InTime; }
	void SetSlowRate(const float& InRate) { Slow_Rate = InRate; }

	/** Returns the actual struct used for serialization, subclasses must override this! */
	virtual UScriptStruct* GetScriptStruct() const
	{
		return FGameplayEffectContext::StaticStruct();
	}

	/** Creates a copy of this context, used to duplicate for later modifications */
	virtual FCubeGameplayEffectContext* Duplicate() const
	{
		FCubeGameplayEffectContext* NewContext = new FCubeGameplayEffectContext();
		*NewContext = *this;
		if (GetHitResult())
		{
			// Does a deep copy of the hit result
			NewContext->AddHitResult(*GetHitResult(), true);
		}
		return NewContext;
	}

	/** Custom serialization, subclasses must override this */
	virtual bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess);

protected:

	//UPROPERTY()
	//bool bIsBlockedHit = false;

	UPROPERTY()
	bool bIsCriticalHit = false;

	UPROPERTY()
	bool bIsPhysicalHit = false;

	UPROPERTY()
	bool bIsMagicalHit = false;

	UPROPERTY()
	bool bIsPureHit = false;

	UPROPERTY()
	bool bIsHealHit = false;

	TSharedPtr<FGameplayTag> DamageType;

	/** Knockback **/
	UPROPERTY() FVector Knockback_Force = FVector::ZeroVector;
	/** end Knockback **/

	/** Grab **/
	UPROPERTY() FVector Grab_Force = FVector::ZeroVector;
	/** end Grab **/

	/** Airborne **/
	UPROPERTY() FVector Airborne_Force = FVector::ZeroVector;
	/** end Airborne **/

	/** Stun **/
	UPROPERTY() float Stun_Time = 0.f;
	/** end Stun **/

	/** Slow **/
	UPROPERTY() float Slow_Time = 0.f;
	UPROPERTY() float Slow_Rate = 0.f; // 0~1
	/** end Slow **/
};

template<>
struct TStructOpsTypeTraits<FCubeGameplayEffectContext> : public TStructOpsTypeTraitsBase2<FCubeGameplayEffectContext>
{
	enum
	{
		WithNetSerializer = true,
		WithCopy = true
	};
};
