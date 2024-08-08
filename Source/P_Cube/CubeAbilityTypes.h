#pragma once

#include "GameplayEffectTypes.h"
#include "CubeAbilityTypes.generated.h"

USTRUCT(BlueprintType)
struct FCubeGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY()

public:

	bool IsCriticalHit() const { return bIsCriticalHit; }
	bool IsPhysicalHit() const { return bIsPhysicalHit; }
	bool IsMagicalHit() const { return bIsMagicalHit; }
	bool IsPureHit() const { return bIsPureHit; }

	void SetIsCriticalHit(bool bInIsCriticalHit) { bIsCriticalHit = bInIsCriticalHit; }
	void SetIsPhysicalHit(bool bInIsPhysicalHit) { bIsPhysicalHit = bInIsPhysicalHit; }
	void SetIsMagicalHit(bool bInIsMagicalHit) { bIsMagicalHit = bInIsMagicalHit; }
	void SetIsPureHit(bool bInIsPureHit) { bIsPureHit = bInIsPureHit; }

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
