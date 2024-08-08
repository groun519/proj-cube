// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "CubeAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)
/*
- GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) : 클래스(ClassName)에서 사용할 attribute(PropertyName) 속성에 대한 접근자를 생성해 속성을 사용할 때 필요한 함수를 정의.
- GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) : 속성(attribute)의 현재 값을 반환하는 getter 함수 생성.
- GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) : 속성(attribute)값을 설정하는 setter 함수 생성.
- GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName) : 속성의 초기값을 설정하는 초기화 함수를 생성. (Init~~())
*/

USTRUCT()
struct FEffectProperties // 이펙트의 속성을 담는 구조체.
{
	GENERATED_BODY()

	FEffectProperties() {}

	FGameplayEffectContextHandle EffectContextHandle;

	UPROPERTY()
	UAbilitySystemComponent* SourceASC = nullptr;

	UPROPERTY()
	AActor* SourceAvatarActor = nullptr;

	UPROPERTY()
	AController* SourceController = nullptr;

	UPROPERTY()
	ACharacter* SourceCharacter = nullptr;

	UPROPERTY()
	UAbilitySystemComponent* TargetASC = nullptr;

	UPROPERTY()
	AActor* TargetAvatarActor = nullptr;

	UPROPERTY()
	AController* TargetController = nullptr; 

	UPROPERTY()
	ACharacter* TargetCharacter = nullptr; 
};

// typedef is specific to the FGameplayAttribute() signature, but TStaticFunPtr is generic to any signature chosen
//typedef TBaseStaticDelegateInstance<FGameplayAttribute(), FDefaultDelegateUserPolicy>::FFuncPtr FAttributeFuncPtr;
template<class T>
using TStaticFuncPtr = typename TBaseStaticDelegateInstance<T, FDefaultDelegateUserPolicy>::FFuncPtr;

/**
 * 
 */
UCLASS()
class P_CUBE_API UCubeAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
public:
	UCubeAttributeSet();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	TMap<FGameplayTag, TStaticFuncPtr<FGameplayAttribute()>> TagsToAttributes;

	/*
	 * Primary Attributes
	 */

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_PhysicalPower, Category = "Primary Attributes")
	FGameplayAttributeData PhysicalPower;
	ATTRIBUTE_ACCESSORS(UCubeAttributeSet, PhysicalPower);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MagicalPower, Category = "Primary Attributes")
	FGameplayAttributeData MagicalPower;
	ATTRIBUTE_ACCESSORS(UCubeAttributeSet, MagicalPower);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Armor, Category = "Primary Attributes")
	FGameplayAttributeData Armor;
	ATTRIBUTE_ACCESSORS(UCubeAttributeSet, Armor);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MagicResistance, Category = "Primary Attributes")
	FGameplayAttributeData MagicResistance;
	ATTRIBUTE_ACCESSORS(UCubeAttributeSet, MagicResistance);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MovementSpeed, Category = "Primary Attributes")
	FGameplayAttributeData MovementSpeed;
	ATTRIBUTE_ACCESSORS(UCubeAttributeSet, MovementSpeed);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CriticalChance, Category = "Primary Attributes")
	FGameplayAttributeData CriticalChance;
	ATTRIBUTE_ACCESSORS(UCubeAttributeSet, CriticalChance);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CriticalDamage, Category = "Primary Attributes")
	FGameplayAttributeData CriticalDamage;
	ATTRIBUTE_ACCESSORS(UCubeAttributeSet, CriticalDamage);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_AttackSpeed, Category = "Primary Attributes")
	FGameplayAttributeData AttackSpeed;
	ATTRIBUTE_ACCESSORS(UCubeAttributeSet, AttackSpeed);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ArmorPenetration, Category = "Primary Attributes")
	FGameplayAttributeData ArmorPenetration;
	ATTRIBUTE_ACCESSORS(UCubeAttributeSet, ArmorPenetration);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ArmorPenetrationRate, Category = "Primary Attributes")
	FGameplayAttributeData ArmorPenetrationRate;
	ATTRIBUTE_ACCESSORS(UCubeAttributeSet, ArmorPenetrationRate);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MagicResistancePenetration, Category = "Primary Attributes")
	FGameplayAttributeData MagicResistancePenetration;
	ATTRIBUTE_ACCESSORS(UCubeAttributeSet, MagicResistancePenetration);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MagicResistancePenetrationRate, Category = "Primary Attributes")
	FGameplayAttributeData MagicResistancePenetrationRate;
	ATTRIBUTE_ACCESSORS(UCubeAttributeSet, MagicResistancePenetrationRate);

	/*
	 * Secondary Attributes
	 */

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ArmorRate, Category = "Secondary Attributes")
	FGameplayAttributeData ArmorRate;
	ATTRIBUTE_ACCESSORS(UCubeAttributeSet, ArmorRate);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MagicResistanceRate, Category = "Secondary Attributes")
	FGameplayAttributeData MagicResistanceRate;
	ATTRIBUTE_ACCESSORS(UCubeAttributeSet, MagicResistanceRate);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MovementSpeedIncreaseRate, Category = "Secondary Attributes")
	FGameplayAttributeData MovementSpeedIncreaseRate;
	ATTRIBUTE_ACCESSORS(UCubeAttributeSet, MovementSpeedIncreaseRate);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_HealthRegeneration, Category = "Secondary Attributes")
	FGameplayAttributeData HealthRegeneration;
	ATTRIBUTE_ACCESSORS(UCubeAttributeSet, HealthRegeneration);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ManaRegeneration, Category = "Secondary Attributes")
	FGameplayAttributeData ManaRegeneration;
	ATTRIBUTE_ACCESSORS(UCubeAttributeSet, ManaRegeneration);

	/*
	 * Vital Attributes
	 */

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category = "Vital Attributes")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UCubeAttributeSet, Health); // Health 속성에 대한 접근자들을 정의함.

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, Category = "Vital Attributes")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UCubeAttributeSet, MaxHealth);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Mana, Category = "Vital Attributes")
	FGameplayAttributeData Mana;
	ATTRIBUTE_ACCESSORS(UCubeAttributeSet, Mana);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxMana, Category = "Vital Attributes")
	FGameplayAttributeData MaxMana;
	ATTRIBUTE_ACCESSORS(UCubeAttributeSet, MaxMana);

	/*
	 * Meta Attributes
	 */

	UPROPERTY(BlueprintReadOnly, Category = "Meta Attributes")
	FGameplayAttributeData IncomingDamage;
	ATTRIBUTE_ACCESSORS(UCubeAttributeSet, IncomingDamage);


	// Vital
	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldHealth) const;

	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const;

	UFUNCTION()
	void OnRep_Mana(const FGameplayAttributeData& OldMana) const;

	UFUNCTION()
	void OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const;

	// Primary
	UFUNCTION()
	void OnRep_PhysicalPower(const FGameplayAttributeData& OldPhysicalPower) const;

	UFUNCTION()
	void OnRep_MagicalPower(const FGameplayAttributeData& OldMagicalPower) const;

	UFUNCTION()
	void OnRep_Armor(const FGameplayAttributeData& OldArmor) const;

	UFUNCTION()
	void OnRep_MagicResistance(const FGameplayAttributeData& OldMagicResistance) const;

	UFUNCTION()
	void OnRep_MovementSpeed(const FGameplayAttributeData& OldMovementSpeed) const;

	UFUNCTION()
	void OnRep_CriticalChance(const FGameplayAttributeData& OldCriticalChance) const;

	UFUNCTION()
	void OnRep_CriticalDamage (const FGameplayAttributeData& OldCriticalDamage) const;

	UFUNCTION()
	void OnRep_AttackSpeed(const FGameplayAttributeData& OldAttackSpeed) const;

	UFUNCTION()
	void OnRep_ArmorPenetration(const FGameplayAttributeData& OldArmorPenetration) const;

	UFUNCTION()
	void OnRep_ArmorPenetrationRate(const FGameplayAttributeData& OldArmorPenetrationRate) const;

	UFUNCTION()
	void OnRep_MagicResistancePenetration(const FGameplayAttributeData& OldMagicResistancePenetration) const;

	UFUNCTION()
	void OnRep_MagicResistancePenetrationRate(const FGameplayAttributeData& OldMagicResistancePenetrationRate) const;

	// Secondary

	UFUNCTION()
	void OnRep_ArmorRate(const FGameplayAttributeData& OldArmorRate) const;

	UFUNCTION()
	void OnRep_MagicResistanceRate(const FGameplayAttributeData& OldMagicResistanceRate) const;

	UFUNCTION()
	void OnRep_MovementSpeedIncreaseRate(const FGameplayAttributeData& OldMovementSpeedIncreaseRate) const;

	UFUNCTION()
	void OnRep_HealthRegeneration(const FGameplayAttributeData& OldHealthRegeneration) const;

	UFUNCTION()
	void OnRep_ManaRegeneration(const FGameplayAttributeData& OldManaRegeneration) const;

private:

	void SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& Props) const;
	void ShowFloatingText(const FEffectProperties& Props, float Damage, bool bCriticalHit, bool bPhysicalHit, bool bMagicalHit, bool bPureHit) const;
};
