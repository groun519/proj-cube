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

	FGameplayTag Attributes_Meta_IncomingXP;

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

	FGameplayTag Abilities_Attack;
	FGameplayTag Abilities_Summon;

	FGameplayTag Abilities_FIreStaff_Attack;
	FGameplayTag Abilities_FIreStaff_Basic;
	FGameplayTag Abilities_FIreStaff_Unique;
	FGameplayTag Abilities_GravityStaff_Attack;
	FGameplayTag Abilities_GravityStaff_Basic;
	FGameplayTag Abilities_GravityStaff_Unique;
	FGameplayTag Abilities_LightStaff_Attack;
	FGameplayTag Abilities_LightStaff_Basic;
	FGameplayTag Abilities_LightStaff_Unique;
	FGameplayTag Abilities_MagicShield_Attack;
	FGameplayTag Abilities_MagicShield_Basic;
	FGameplayTag Abilities_MagicShield_Unique;
	FGameplayTag Abilities_HugeShield_Attack;
	FGameplayTag Abilities_HugeShield_Basic;
	FGameplayTag Abilities_HugeShield_Unique;
	FGameplayTag Abilities_Rappier_Attack;
	FGameplayTag Abilities_Rappier_Basic;
	FGameplayTag Abilities_Rappier_Unique;
	FGameplayTag Abilities_ShortBow_Attack;
	FGameplayTag Abilities_ShortBow_Basic;
	FGameplayTag Abilities_ShortBow_Unique;
	FGameplayTag Abilities_Revolver_Attack;
	FGameplayTag Abilities_Revolver_Basic;
	FGameplayTag Abilities_Revolver_Unique;
	FGameplayTag Abilities_LongBow_Attack;
	FGameplayTag Abilities_LongBow_Basic;
	FGameplayTag Abilities_LongBow_Unique;

	FGameplayTag Cooldown_FIreStaff_Attack;
	FGameplayTag Cooldown_FIreStaff_Basic;
	FGameplayTag Cooldown_FIreStaff_Unique;
	FGameplayTag Cooldown_GravityStaff_Attack;
	FGameplayTag Cooldown_GravityStaff_Basic;
	FGameplayTag Cooldown_GravityStaff_Unique;
	FGameplayTag Cooldown_LightStaff_Attack;
	FGameplayTag Cooldown_LightStaff_Basic;
	FGameplayTag Cooldown_LightStaff_Unique;
	FGameplayTag Cooldown_MagicShield_Attack;
	FGameplayTag Cooldown_MagicShield_Basic;
	FGameplayTag Cooldown_MagicShield_Unique;
	FGameplayTag Cooldown_HugeShield_Attack;
	FGameplayTag Cooldown_HugeShield_Basic;
	FGameplayTag Cooldown_HugeShield_Unique;
	FGameplayTag Cooldown_Rappier_Attack;
	FGameplayTag Cooldown_Rappier_Basic;
	FGameplayTag Cooldown_Rappier_Unique;
	FGameplayTag Cooldown_ShortBow_Attack;
	FGameplayTag Cooldown_ShortBow_Basic;
	FGameplayTag Cooldown_ShortBow_Unique;
	FGameplayTag Cooldown_Revolver_Attack;
	FGameplayTag Cooldown_Revolver_Basic;
	FGameplayTag Cooldown_Revolver_Unique;
	FGameplayTag Cooldown_LongBow_Attack;
	FGameplayTag Cooldown_LongBow_Basic;
	FGameplayTag Cooldown_LongBow_Unique;

	FGameplayTag CombatSocket_ActorLocation;
	FGameplayTag CombatSocket_Weapon;
	FGameplayTag CombatSocket_RightHand;
	FGameplayTag CombatSocket_LeftHand;

	FGameplayTag Montage_Attack_1;
	FGameplayTag Montage_Attack_2;
	FGameplayTag Montage_Attack_3;
	FGameplayTag Montage_Attack_4;

	TArray<FGameplayTag> DamageTypes;

	FGameplayTag Effects_HitReact;
private:
	static FCubeGameplayTags GameplayTags;
};
