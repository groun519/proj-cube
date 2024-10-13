// Fill out your copyright notice in the Description page of Project Settings.


#include "CubeGameplayTags.h"
#include "GameplayTagsManager.h"


FCubeGameplayTags FCubeGameplayTags::GameplayTags;

void FCubeGameplayTags::InitializeNativeGameplayTags()
{
	/*
	 * Primary Attributes
	 */

	GameplayTags.Attributes_Primary_PhysicalPower = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.PhysicalPower"),
		FString("Increases physical damage")
	);

	GameplayTags.Attributes_Primary_MagicalPower = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.MagicalPower"),
		FString("Increases magical damage")
	);

	GameplayTags.Attributes_Primary_Armor = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.Armor"),
		FString("Increases Armor")
	);

	GameplayTags.Attributes_Primary_MagicResistance = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.MagicResistance"),
		FString("Increases Magical Armor")
	);

	GameplayTags.Attributes_Primary_MovementSpeed = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.MovementSpeed"),
		FString("Increases Movement Speed")
	);

	GameplayTags.Attributes_Primary_CriticalChance = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.CriticalChance"),
		FString("Increases Critical Attack Chance")
	);

	GameplayTags.Attributes_Primary_CriticalDamage = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.CriticalDamage"),
		FString("Increases Critical Attack Damage")
	);

	GameplayTags.Attributes_Primary_AttackSpeed = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.AttackSpeed"),
		FString("Increases Attack Speed")
	);

	GameplayTags.Attributes_Primary_ArmorPenetration = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.ArmorPenetration"),
		FString("Increases Armor Penetration")
	);

	GameplayTags.Attributes_Primary_ArmorPenetrationRate = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.ArmorPenetrationRate"),
		FString("Increases Armor Penetration Rate")
	);

	GameplayTags.Attributes_Primary_MagicResistancePenetration = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.MagicResistancePenetration"),
		FString("Increases Magic Armor Penetration")
	);

	GameplayTags.Attributes_Primary_MagicResistancePenetrationRate = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.MagicResistancePenetrationRate"),
		FString("Increases Magic Armor Penetration Rate")
	);

	/*
	 * Secondary Attributes
	 */

	GameplayTags.Attributes_Secondary_ArmorRate = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.ArmorRate"),
		FString("Reduces Physical damage taken, reduces physical damage taken")
	);

	GameplayTags.Attributes_Secondary_MagicResistanceRate = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.MagicResistanceRate"),
		FString("Reduces Magiacal damage taken, reduces magical damage taken")
	);

	GameplayTags.Attributes_Secondary_MovementSpeedIncreaseRate = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.MovementSpeedRate"),
		FString("Multiplier of Movement Speed and Animation Speed")
	);

	GameplayTags.Attributes_Secondary_HealthRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.HealthRegeneration"),
		FString("Amount of Health regenerated every 1 second")
	);

	GameplayTags.Attributes_Secondary_ManaRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.ManaRegeneration"),
		FString("Amount of Mana regenerated every 1 second")
	);

	GameplayTags.Attributes_Secondary_MaxHealth = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.MaxHealth"),
		FString("Maximum amount of Health obtainable")
	);

	GameplayTags.Attributes_Secondary_MaxMana = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.MaxMana"),
		FString("Maximum amount of Mana obtainable")
	);

	/*
	 * Input Tags
	 */

	GameplayTags.InputTag_LMB = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.LMB"),
		FString("Input Tag for Left Mouse Button")
	);

	GameplayTags.InputTag_RMB = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.RMB"),
		FString("Input Tag for Right Mouse Button")
	);

	GameplayTags.InputTag_Q = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.Q"),
		FString("Input Tag for Q key")
	);

	GameplayTags.InputTag_W = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.W"),
		FString("Input Tag for W key")
	);

	GameplayTags.InputTag_E = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.E"),
		FString("Input Tag for E key")
	);

	GameplayTags.InputTag_R = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.R"),
		FString("Input Tag for R key")
	);

	GameplayTags.InputTag_T = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.T"),
		FString("Input Tag for T key")
	);

	GameplayTags.InputTag_D = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.D"),
		FString("Input Tag for D key")
	);

	GameplayTags.InputTag_F = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.F"),
		FString("Input Tag for F key")
	);

	GameplayTags.InputTag_G = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.G"),
		FString("Input Tag for G key")
	);

	GameplayTags.InputTag_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.1"),
		FString("Input Tag for 1 key")
	);

	GameplayTags.InputTag_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.2"),
		FString("Input Tag for 2 key")
	);

	GameplayTags.InputTag_3 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.3"),
		FString("Input Tag for 3 key")
	);

	GameplayTags.InputTag_4 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.4"),
		FString("Input Tag for 4 key")
	);

	GameplayTags.InputTag_S = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.S"),
		FString("Input Tag for S key")
	);

	GameplayTags.Damage = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Damage"),
		FString("Damage")
	);

	/*
	 * Damage Types
	 */

	GameplayTags.Damage_Physical = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Damage.Physical"),
		FString("Physical Damage Type")
	);
	GameplayTags.Damage_Magical = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Damage.Magical"),
		FString("Magical Damage Type")
	);
	GameplayTags.Damage_Pure = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Damage.Pure"),
		FString("Pure Damage Type")
	);

	/*
	 * Meta Attributes
	 */

	GameplayTags.Attributes_Meta_IncomingXP = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Meta.IncomingXP"),
		FString("Incoming XP Meta Attribute")
	);

	/*
	 * Array of Damage Types
	 */

	GameplayTags.DamageTypes.Add(GameplayTags.Damage_Physical);
	GameplayTags.DamageTypes.Add(GameplayTags.Damage_Magical);
	GameplayTags.DamageTypes.Add(GameplayTags.Damage_Pure);

	/*
	 * Effects
	 */

	GameplayTags.Effects_HitReact = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Effects.HitReact"),
		FString("Tag granted when Hit Reacting")
	);

	/*
	 * Abilities
	 */

	GameplayTags.Abilities_None = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.None"),
		FString("No Ability - like the nullptr for Ability Tags")
	);

	GameplayTags.Abilities_Attack = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Attack"),
		FString("Attack Ability Tag")
	);

	GameplayTags.Abilities_Summon = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Summon"),
		FString("Summon Ability Tag")
	);




	GameplayTags.Abilities_HitReact = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.HitReact"),
		FString("Hit React Ability")
	);

	GameplayTags.Abilities_Status_Locked = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Status.Locked"),
		FString("Locked Status")
	);

	GameplayTags.Abilities_Status_Unlocked = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Status.Unlocked"),
		FString("Unlocked Status")
	);

	GameplayTags.Abilities_Status_Fixed = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Status.Fixed"),
		FString("Fixed Status")
	);

	GameplayTags.Abilities_Status_LowLevel = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Status.LowLevel"),
		FString("Low Level Status")
	);

	GameplayTags.Abilities_Type_None = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Type.None"),
		FString("Type None")
	);

	GameplayTags.Abilities_Type_Offensive = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Type.Offensive"),
		FString("Type Offensive")
	);

	GameplayTags.Abilities_Type_Passive = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Type.Passive"),
		FString("Type Passive")
	);




	GameplayTags.Abilities_FIreStaff_Attack = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.FIreStaff.Attack"),
		FString("FireStaff's Attack Ability Tag")
	);
	GameplayTags.Abilities_FIreStaff_Basic = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.FIreStaff.Basic"),
		FString("FireStaff's Basic Ability Tag")
	);
	GameplayTags.Abilities_FIreStaff_Unique = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.FIreStaff.Unique"),
		FString("FireStaff's Unique Ability Tag")
	);

	GameplayTags.Abilities_GravityStaff_Attack = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.GravityStaff.Attack"),
		FString("GravityStaff's Attack Ability Tag")
	);
	GameplayTags.Abilities_GravityStaff_Basic = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.GravityStaff.Basic"),
		FString("GravityStaff's Basic Ability Tag")
	);
	GameplayTags.Abilities_GravityStaff_Unique = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.GravityStaff.Unique"),
		FString("GravityStaff's Unique Ability Tag")
	);

	GameplayTags.Abilities_LightStaff_Attack = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.LightStaff.Attack"),
		FString("LightStaff's Attack Ability Tag")
	);
	GameplayTags.Abilities_LightStaff_Basic = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.LightStaff.Basic"),
		FString("LightStaff's Basic Ability Tag")
	);
	GameplayTags.Abilities_LightStaff_Unique = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.LightStaff.Unique"),
		FString("LightStaff's Unique Ability Tag")
	);

	GameplayTags.Abilities_MagicShield_Attack = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.MagicShield.Attack"),
		FString("MagicShield's Attack Ability Tag")
	);
	GameplayTags.Abilities_MagicShield_Basic = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.MagicShield.Basic"),
		FString("MagicShield's Basic Ability Tag")
	);
	GameplayTags.Abilities_MagicShield_Unique = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.MagicShield.Unique"),
		FString("MagicShield's Unique Ability Tag")
	);

	GameplayTags.Abilities_HugeShield_Attack = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.HugeShield.Attack"),
		FString("HugeShield's Attack Ability Tag")
	);
	GameplayTags.Abilities_HugeShield_Basic = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.HugeShield.Basic"),
		FString("HugeShield's Basic Ability Tag")
	);
	GameplayTags.Abilities_HugeShield_Unique = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.HugeShield.Unique"),
		FString("HugeShield's Unique Ability Tag")
	);

	GameplayTags.Abilities_Rappier_Attack = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Rappier.Attack"),
		FString("Rappier's Attack Ability Tag")
	);
	GameplayTags.Abilities_Rappier_Basic = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Rappier.Basic"),
		FString("Rappier's Basic Ability Tag")
	);
	GameplayTags.Abilities_Rappier_Unique = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Rappier.Unique"),
		FString("Rappier's Unique Ability Tag")
	);

	GameplayTags.Abilities_ShortBow_Attack = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.ShortBow.Attack"),
		FString("ShortBow's Attack Ability Tag")
	);
	GameplayTags.Abilities_ShortBow_Basic = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.ShortBow.Basic"),
		FString("ShortBow's Basic Ability Tag")
	);
	GameplayTags.Abilities_ShortBow_Unique = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.ShortBow.Unique"),
		FString("ShortBow's Unique Ability Tag")
	);

	GameplayTags.Abilities_Revolver_Attack = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Revolver.Attack"),
		FString("Revolver's Attack Ability Tag")
	);
	GameplayTags.Abilities_Revolver_Basic = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Revolver.Basic"),
		FString("Revolver's Basic Ability Tag")
	);
	GameplayTags.Abilities_Revolver_Unique = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Revolver.Unique"),
		FString("Revolver's Unique Ability Tag")
	);

	GameplayTags.Abilities_SkullStaff_Attack = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.SkullStaff.Attack"),
		FString("SkullStaff's Attack Ability Tag")
	);
	GameplayTags.Abilities_SkullStaff_Basic = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.SkullStaff.Basic"),
		FString("SkullStaff's Basic Ability Tag")
	);
	GameplayTags.Abilities_SkullStaff_Unique = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.SkullStaff.Unique"),
		FString("SkullStaff's Unique Ability Tag")
	);

	GameplayTags.Abilities_Ninja_Attack = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Ninja.Attack"),
		FString("Ninja's Attack Ability Tag")
	);
	GameplayTags.Abilities_Ninja_Basic = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Ninja.Basic"),
		FString("Ninja's Basic Ability Tag")
	);
	GameplayTags.Abilities_Ninja_Unique = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Ninja.Unique"),
		FString("Ninja's Unique Ability Tag")
	);

	/*GameplayTags.AbilityTags.Add(GameplayTags.Abilities_FIreStaff_Attack);
	GameplayTags.AbilityTags.Add(GameplayTags.Abilities_FIreStaff_Basic);
	GameplayTags.AbilityTags.Add(GameplayTags.Abilities_FIreStaff_Unique);
	GameplayTags.AbilityTags.Add(GameplayTags.Abilities_GravityStaff_Attack);
	GameplayTags.AbilityTags.Add(GameplayTags.Abilities_GravityStaff_Basic);
	GameplayTags.AbilityTags.Add(GameplayTags.Abilities_GravityStaff_Unique);
	GameplayTags.AbilityTags.Add(GameplayTags.Abilities_LightStaff_Attack);
	GameplayTags.AbilityTags.Add(GameplayTags.Abilities_LightStaff_Basic);
	GameplayTags.AbilityTags.Add(GameplayTags.Abilities_LightStaff_Unique);
	GameplayTags.AbilityTags.Add(GameplayTags.Abilities_MagicShield_Attack);
	GameplayTags.AbilityTags.Add(GameplayTags.Abilities_MagicShield_Basic);
	GameplayTags.AbilityTags.Add(GameplayTags.Abilities_MagicShield_Unique);
	GameplayTags.AbilityTags.Add(GameplayTags.Abilities_HugeShield_Attack);
	GameplayTags.AbilityTags.Add(GameplayTags.Abilities_HugeShield_Basic);
	GameplayTags.AbilityTags.Add(GameplayTags.Abilities_HugeShield_Unique);
	GameplayTags.AbilityTags.Add(GameplayTags.Abilities_Rappier_Attack);
	GameplayTags.AbilityTags.Add(GameplayTags.Abilities_Rappier_Basic);
	GameplayTags.AbilityTags.Add(GameplayTags.Abilities_Rappier_Unique);
	GameplayTags.AbilityTags.Add(GameplayTags.Abilities_ShortBow_Attack);
	GameplayTags.AbilityTags.Add(GameplayTags.Abilities_ShortBow_Basic);
	GameplayTags.AbilityTags.Add(GameplayTags.Abilities_ShortBow_Unique);
	GameplayTags.AbilityTags.Add(GameplayTags.Abilities_Revolver_Attack);
	GameplayTags.AbilityTags.Add(GameplayTags.Abilities_Revolver_Basic);
	GameplayTags.AbilityTags.Add(GameplayTags.Abilities_Revolver_Unique);
	GameplayTags.AbilityTags.Add(GameplayTags.Abilities_SkullStaff_Attack);
	GameplayTags.AbilityTags.Add(GameplayTags.Abilities_SkullStaff_Basic);
	GameplayTags.AbilityTags.Add(GameplayTags.Abilities_SkullStaff_Unique);
	GameplayTags.AbilityTags.Add(GameplayTags.Abilities_Ninja_Attack);
	GameplayTags.AbilityTags.Add(GameplayTags.Abilities_Ninja_Basic);
	GameplayTags.AbilityTags.Add(GameplayTags.Abilities_Ninja_Unique);*/


	/*
	 * Cooldown
	 */

	GameplayTags.Cooldown_FIreStaff_Attack = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.FIreStaff.Attack"),
		FString("FireStaff's Attack Cooldown Tag")
	);
	GameplayTags.Cooldown_FIreStaff_Basic = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.FIreStaff.Basic"),
		FString("FireStaff's Basic Cooldown Tag")
	);
	GameplayTags.Cooldown_FIreStaff_Unique = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.FIreStaff.Unique"),
		FString("FireStaff's Unique Cooldown Tag")
	);

	GameplayTags.Cooldown_GravityStaff_Attack = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.GravityStaff.Attack"),
		FString("GravityStaff's Attack Cooldown Tag")
	);
	GameplayTags.Cooldown_GravityStaff_Basic = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.GravityStaff.Basic"),
		FString("GravityStaff's Basic Cooldown Tag")
	);
	GameplayTags.Cooldown_GravityStaff_Unique = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.GravityStaff.Unique"),
		FString("GravityStaff's Unique Cooldown Tag")
	);

	GameplayTags.Cooldown_LightStaff_Attack = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.LightStaff.Attack"),
		FString("LightStaff's Attack Cooldown Tag")
	);
	GameplayTags.Cooldown_LightStaff_Basic = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.LightStaff.Basic"),
		FString("LightStaff's Basic Cooldown Tag")
	);
	GameplayTags.Cooldown_LightStaff_Unique = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.LightStaff.Unique"),
		FString("LightStaff's Unique Cooldown Tag")
	);

	GameplayTags.Cooldown_MagicShield_Attack = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.MagicShield.Attack"),
		FString("MagicShield's Attack Cooldown Tag")
	);
	GameplayTags.Cooldown_MagicShield_Basic = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.MagicShield.Basic"),
		FString("MagicShield's Basic Cooldown Tag")
	);
	GameplayTags.Cooldown_MagicShield_Unique = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.MagicShield.Unique"),
		FString("MagicShield's Unique Cooldown Tag")
	);

	GameplayTags.Cooldown_HugeShield_Attack = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.HugeShield.Attack"),
		FString("HugeShield's Attack Cooldown Tag")
	);
	GameplayTags.Cooldown_HugeShield_Basic = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.HugeShield.Basic"),
		FString("HugeShield's Basic Cooldown Tag")
	);
	GameplayTags.Cooldown_HugeShield_Unique = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.HugeShield.Unique"),
		FString("HugeShield's Unique Cooldown Tag")
	);

	GameplayTags.Cooldown_Rappier_Attack = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.Rappier.Attack"),
		FString("Rappier's Attack Cooldown Tag")
	);
	GameplayTags.Cooldown_Rappier_Basic = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.Rappier.Basic"),
		FString("Rappier's Basic Cooldown Tag")
	);
	GameplayTags.Cooldown_Rappier_Unique = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.Rappier.Unique"),
		FString("Rappier's Unique Cooldown Tag")
	);

	GameplayTags.Cooldown_ShortBow_Attack = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.ShortBow.Attack"),
		FString("ShortBow's Attack Cooldown Tag")
	);
	GameplayTags.Cooldown_ShortBow_Basic = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.ShortBow.Basic"),
		FString("ShortBow's Basic Cooldown Tag")
	);
	GameplayTags.Cooldown_ShortBow_Unique = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.ShortBow.Unique"),
		FString("ShortBow's Unique Cooldown Tag")
	);

	GameplayTags.Cooldown_Revolver_Attack = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.Revolver.Attack"),
		FString("Revolver's Attack Cooldown Tag")
	);
	GameplayTags.Cooldown_Revolver_Basic = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.Revolver.Basic"),
		FString("Revolver's Basic Cooldown Tag")
	);
	GameplayTags.Cooldown_Revolver_Unique = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.Revolver.Unique"),
		FString("Revolver's Unique Cooldown Tag")
	);

	GameplayTags.Cooldown_SkullStaff_Attack = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.SkullStaff.Attack"),
		FString("SkullStaff's Attack Cooldown Tag")
	);
	GameplayTags.Cooldown_SkullStaff_Basic = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.SkullStaff.Basic"),
		FString("SkullStaff's Basic Cooldown Tag")
	);
	GameplayTags.Cooldown_SkullStaff_Unique = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.SkullStaff.Unique"),
		FString("SkullStaff's Unique Cooldown Tag")
	);

	GameplayTags.Cooldown_Ninja_Attack = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.Ninja.Attack"),
		FString("Ninja's Attack Cooldown Tag")
	);
	GameplayTags.Cooldown_Ninja_Basic = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.Ninja.Basic"),
		FString("Ninja's Basic Cooldown Tag")
	);
	GameplayTags.Cooldown_Ninja_Unique = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.Ninja.Unique"),
		FString("Ninja's Unique Cooldown Tag")
	);

	/*
	 * Combat Sockets
	 */

	GameplayTags.CombatSocket_ActorLocation = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("CombatSocket.ActorLocation"),
		FString("Actor Location")
	);

	GameplayTags.CombatSocket_Weapon = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("CombatSocket.Weapon"),
		FString("Weapon")
	);

	GameplayTags.CombatSocket_RightHand = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("CombatSocket.RightHand"),
		FString("Right Hand")
	);

	GameplayTags.CombatSocket_LeftHand = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("CombatSocket.LeftHand"),
		FString("Left Hand")
	);

	/*
	 * Montage Tags
	 */

	GameplayTags.Montage_Attack_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Montage.Attack.1"),
		FString("Attack 1")
	);

	GameplayTags.Montage_Attack_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Montage.Attack.2"),
		FString("Attack 2")
	);

	GameplayTags.Montage_Attack_3 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Montage.Attack.3"),
		FString("Attack 3")
	);

	GameplayTags.Montage_Attack_4 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Montage.Attack.4"),
		FString("Attack 4")
	);
}
