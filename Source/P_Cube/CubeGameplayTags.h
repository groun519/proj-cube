// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

/**
 * CubeGameplayTags
 *
 * Singleton containing native Gameplay Tags
 */

struct FCubeGameplayTags
{
public:
	static const FCubeGameplayTags& Get() { return GameplayTags; }
	static void InitializeNativeGameplayTags();

	FGameplayTag Attributes_Primary_PhysicalPower;
	FGameplayTag Attributes_Primary_MagicalPower;
	FGameplayTag Attributes_Primary_Armor;
	FGameplayTag Attributes_Primary_MagicResistance;
	FGameplayTag Attributes_Primary_MovementSpeed;
	FGameplayTag Attributes_Primary_CriticalChance;
	FGameplayTag Attributes_Primary_CriticalDamage;
	FGameplayTag Attributes_Primary_AttackSpeed;
	FGameplayTag Attributes_Primary_ArmorPenetration;
	FGameplayTag Attributes_Primary_ArmorPenetrationRate;
	FGameplayTag Attributes_Primary_MagicResistancePenetration;
	FGameplayTag Attributes_Primary_MagicResistancePenetrationRate;

	FGameplayTag Attributes_Secondary_ArmorRate;
	FGameplayTag Attributes_Secondary_MagicResistanceRate;
	FGameplayTag Attributes_Secondary_MovementSpeedIncreaseRate;
	FGameplayTag Attributes_Secondary_HealthRegeneration;
	FGameplayTag Attributes_Secondary_ManaRegeneration;
	FGameplayTag Attributes_Secondary_MaxHealth;
	FGameplayTag Attributes_Secondary_MaxMana;

	FGameplayTag InputTag_LMB;
	FGameplayTag InputTag_RMB;
	FGameplayTag InputTag_Q;
	FGameplayTag InputTag_W;
	FGameplayTag InputTag_E;
	FGameplayTag InputTag_R;
	FGameplayTag InputTag_D;
	FGameplayTag InputTag_F;

	FGameplayTag Damage;
	FGameplayTag Damage_Physical;
	FGameplayTag Damage_Magical;
	FGameplayTag Damage_Pure;

	TArray<FGameplayTag> DamageTypes;

	FGameplayTag Effects_HitReact;
private:
	static FCubeGameplayTags GameplayTags;
};
