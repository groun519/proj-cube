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

	GameplayTags.InputTag_D = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.D"),
		FString("Input Tag for D key")
	);

	GameplayTags.InputTag_F = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.F"),
		FString("Input Tag for F key")
	);
}
