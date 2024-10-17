// Fill out your copyright notice in the Description page of Project Settings.


#include "P_Cube/AbilitySystem/Abilities/CubeFireStaff.h"
#include "P_Cube/CubeGameplayTags.h"

FString UCubeFireStaff::GetBasicDescription(int32 Level)
{
	const int32 Damage = GetDamageByDamageType(Level, FCubeGameplayTags::Get().Damage_Magical);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	if (Level == 1)
	{
		return FString::Printf(TEXT(
			// Title
			"<Title>FIRE BOLT</>\n\n"
			// Level
			"<Small>Level: </><Level>%d</>\n"
			// ManaCost
			"<Small>ManaCost: </><ManaCost>%.1f</>\n"
			// Cooldown
			"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"

			"<Default>Launches a bolt of fire, "
			"exploding on impact and dealing: </>"
			// Damage
			"<Damage>%d</><Default> fire damage with"
			" a chance to burn</>"),
			// Values
			Level,
			ManaCost,
			Cooldown,
			Damage);
	}
	else
	{
		return FString::Printf(TEXT(
			// Title
			"<Title>FIRE BOLT</>\n\n"
			// Level
			"<Small>Level: </><Level>%d</>\n"
			// ManaCost
			"<Small>ManaCost: </><ManaCost>%.1f</>\n"
			// Cooldown
			"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"
			// Number of FireBolts
			"<Default>Launches bolts of fire, "
			"exploding on impact and dealing: </>"
			// Damage
			"<Damage>%d</><Default> fire damage with"
			" a chance to burn</>"),
			// Values
			Level,
			ManaCost,
			Cooldown,
			Damage);
	}
}

FString UCubeFireStaff::GetDetailedBasicDescription(int32 Level)
{
	const int32 Damage = GetDamageByDamageType(Level, FCubeGameplayTags::Get().Damage_Magical);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	if (Level == 1)
	{
		return FString::Printf(TEXT(
			// Title
			"<Title>불 스태프</>\n\n"
			// Level
			"<Small>Level: </><Level>%d</>\n"
			// ManaCost
			"<Small>ManaCost: </><ManaCost>%.1f</>\n"
			// Cooldown
			"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"

			"<Default>Launches a bolt of fire, "
			"exploding on impact and dealing: </>"
			// Damage
			"<Damage>%d</><Default> fire damage with"
			" a chance to burn</>"),
			// Values
			Level,
			ManaCost,
			Cooldown,
			Damage);
	}
	else
	{
		return FString::Printf(TEXT(
			// Title
			"<Title>FIRE BOLT</>\n\n"
			// Level
			"<Small>Level: </><Level>%d</>\n"
			// ManaCost
			"<Small>ManaCost: </><ManaCost>%.1f</>\n"
			// Cooldown
			"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"
			// Number of FireBolts
			"<Default>Launches bolts of fire, "
			"exploding on impact and dealing: </>"
			// Damage
			"<Damage>%d</><Default> fire damage with"
			" a chance to burn</>"),
			// Values
			Level,
			ManaCost,
			Cooldown,
			Damage);
	}
}

FString UCubeFireStaff::GetUniqueDescription(int32 Level)
{
	const int32 Damage = GetDamageByDamageType(Level, FCubeGameplayTags::Get().Damage_Magical);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	if (Level == 1)
	{
		return FString::Printf(TEXT(
			// Title
			"<Title>FIRE BOLT</>\n\n"
			// Level
			"<Small>Level: </><Level>%d</>\n"
			// ManaCost
			"<Small>ManaCost: </><ManaCost>%.1f</>\n"
			// Cooldown
			"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"

			"<Default>Launches a bolt of fire, "
			"exploding on impact and dealing: </>"
			// Damage
			"<Damage>%d</><Default> fire damage with"
			" a chance to burn</>"),
			// Values
			Level,
			ManaCost,
			Cooldown,
			Damage);
	}
	else
	{
		return FString::Printf(TEXT(
			// Title
			"<Title>FIRE BOLT</>\n\n"
			// Level
			"<Small>Level: </><Level>%d</>\n"
			// ManaCost
			"<Small>ManaCost: </><ManaCost>%.1f</>\n"
			// Cooldown
			"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"
			// Number of FireBolts
			"<Default>Launches bolts of fire, "
			"exploding on impact and dealing: </>"
			// Damage
			"<Damage>%d</><Default> fire damage with"
			" a chance to burn</>"),
			// Values
			Level,
			ManaCost,
			Cooldown,
			Damage);
	}
}

FString UCubeFireStaff::GetDetailedUniqueDescription(int32 Level)
{
	const int32 Damage = GetDamageByDamageType(Level, FCubeGameplayTags::Get().Damage_Magical);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	if (Level == 1)
	{
		return FString::Printf(TEXT(
			// Title
			"<Title>FIRE BOLT</>\n\n"
			// Level
			"<Small>Level: </><Level>%d</>\n"
			// ManaCost
			"<Small>ManaCost: </><ManaCost>%.1f</>\n"
			// Cooldown
			"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"

			"<Default>Launches a bolt of fire, "
			"exploding on impact and dealing: </>"
			// Damage
			"<Damage>%d</><Default> fire damage with"
			" a chance to burn</>"),
			// Values
			Level,
			ManaCost,
			Cooldown,
			Damage);
	}
	else
	{
		return FString::Printf(TEXT(
			// Title
			"<Title>FIRE BOLT</>\n\n"
			// Level
			"<Small>Level: </><Level>%d</>\n"
			// ManaCost
			"<Small>ManaCost: </><ManaCost>%.1f</>\n"
			// Cooldown
			"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"
			// Number of FireBolts
			"<Default>Launches bolts of fire, "
			"exploding on impact and dealing: </>"
			// Damage
			"<Damage>%d</><Default> fire damage with"
			" a chance to burn</>"),
			// Values
			Level,
			ManaCost,
			Cooldown,
			Damage);
	}
}
