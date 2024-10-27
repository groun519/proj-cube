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

	UPROPERTY(/*BlueprintReadWrite, EditAnywhere, Category = "DamageEffectParams"*/)
	TObjectPtr<UObject> WorldContextObject = nullptr;

	UPROPERTY()
	TSubclassOf<UGameplayEffect> DamageGameplayEffectClass = nullptr;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> SourceAbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> TargetAbilitySystemComponent;

	


	UPROPERTY()
	float AbilityLevel = 1.f;

	UPROPERTY()
	FGameplayTag DamageType = FGameplayTag();

	UPROPERTY()
	float BaseDamage = 0.f;

	UPROPERTY()
	TArray<FCoeffs> AttributeCoeffs;



	UPROPERTY()
	bool bKnockback = false;

	UPROPERTY()
	float KnockbackForce = 0.f;


	/*UPROPERTY()
	float Duration;

	UPROPERTY()
	float Frequency;*/
};

USTRUCT(BlueprintType)
struct FCubeGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY()

public:

	bool IsCriticalHit() const { return bIsCriticalHit; }
	bool IsPhysicalHit() const { return bIsPhysicalHit; }
	bool IsMagicalHit() const { return bIsMagicalHit; }
	bool IsPureHit() const { return bIsPureHit; }
	bool IsSuccessfulDebuff() const { return bIsSuccessfulDebuff; }
	TSharedPtr<FGameplayTag> GetDamageType() const
	{
		return DamageType;
	}

	void SetIsCriticalHit(bool bInIsCriticalHit) { bIsCriticalHit = bInIsCriticalHit; }
	void SetIsPhysicalHit(bool bInIsPhysicalHit) { bIsPhysicalHit = bInIsPhysicalHit; }
	void SetIsMagicalHit(bool bInIsMagicalHit) { bIsMagicalHit = bInIsMagicalHit; }
	void SetIsPureHit(bool bInIsPureHit) { bIsPureHit = bInIsPureHit; }
	void SetIsSuccessfulDebuff(bool bInIsDebuff) { bIsSuccessfulDebuff = bInIsDebuff; }
	void SetDamageType(TSharedPtr<FGameplayTag> InDamageType)
	{
		DamageType = InDamageType;
	}

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
	bool bIsSuccessfulDebuff = false;

	TSharedPtr<FGameplayTag> DamageType;

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
