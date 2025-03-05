// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

/**
 * CubeGameplayTags
 *
 * Singleton containing native Gameplay Tags
 */

struct FResistanceInfo
{
	FResistanceInfo(FGameplayTag Resist, FGameplayTag ResistRate, FGameplayTag Penet, FGameplayTag PenetRate)
	{
		Resistance = Resist;
		ResistanceRate = ResistRate;
		Penetration = Penet;
		PenetrationRate = PenetRate;
	}

	FGameplayTag Resistance;

	FGameplayTag ResistanceRate;

	FGameplayTag Penetration;

	FGameplayTag PenetrationRate;
};

struct FDebuffInfo
{
	FDebuffInfo(FGameplayTag Type, float Dam, float Dur, float Fre)
	{
		DamageType = Type;
		Damage = Dam;
		Duration = Dur;
		Frequency = Fre;
	}

	FGameplayTag DamageType;

	float Damage;

	float Duration;

	float Frequency;
};

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
	FGameplayTag Attributes_Primary_CooldownReduction;
	FGameplayTag Attributes_Primary_Range;

	FGameplayTag Attributes_Secondary_ArmorRate;
	FGameplayTag Attributes_Secondary_MagicResistanceRate;
	FGameplayTag Attributes_Secondary_MovementSpeedIncreaseRate;
	FGameplayTag Attributes_Secondary_HealthRegeneration;
	FGameplayTag Attributes_Secondary_ManaRegeneration;
	FGameplayTag Attributes_Secondary_MaxHealth;
	FGameplayTag Attributes_Secondary_MaxMana;
	FGameplayTag Attributes_Secondary_CooldownReductionRate;

	FGameplayTag Attributes_Vital_Health;
	FGameplayTag Attributes_Vital_Mana;

	FGameplayTag Attributes_Meta_IncomingXP;

	FGameplayTag InputTag_LMB;
	FGameplayTag InputTag_RMB;
	FGameplayTag InputTag_Q;
	FGameplayTag InputTag_W;
	FGameplayTag InputTag_E;
	FGameplayTag InputTag_R;
	FGameplayTag InputTag_T;
	FGameplayTag InputTag_D;
	FGameplayTag InputTag_F;
	FGameplayTag InputTag_G;
	FGameplayTag InputTag_1;
	FGameplayTag InputTag_2;
	FGameplayTag InputTag_3;
	FGameplayTag InputTag_4;
	FGameplayTag InputTag_S;

	FGameplayTag Damage;
	FGameplayTag Damage_Physical;
	FGameplayTag Damage_Magical;
	FGameplayTag Damage_Pure;
	FGameplayTag Damage_Heal;

	FGameplayTag BlockCritical;

	FGameplayTag Debuff_Burn;
	FGameplayTag Debuff_Stun;

	FGameplayTag Debuff_Damage;
	FGameplayTag Debuff_Duration;
	FGameplayTag Debuff_Frequency;

	FGameplayTag Abilities_None;

	FGameplayTag Abilities_Summon;

	FGameplayTag Abilities_HitReact;

	FGameplayTag Abilities_Status_Locked;
	FGameplayTag Abilities_Status_UnLocked;
	FGameplayTag Abilities_Status_Equipped;

	FGameplayTag Abilities_Type_Offensive;
	FGameplayTag Abilities_Type_Passive;
	FGameplayTag Abilities_Type_Scroll;
	FGameplayTag Abilities_Type_None;

	/** Rune Tags **/
	FGameplayTag Rune_Specialization; // TODO ? 얘는 룬이 아니라 기존 유니크시스템 보존용도로 특수 '젬'으로 뺄까 생각중
	
	FGameplayTag Rune_Size_Increase;
	FGameplayTag Rune_Count_Increase;
	FGameplayTag Rune_Repeat;
	FGameplayTag Rune_Damage_Increase;
	FGameplayTag Rune_Speed_Increase;
	FGameplayTag Rune_Duration_Increase;
	/** end Rune Tags **/

	/** Offensive Skill Ability Tags **/
	FGameplayTag Abilities_Attack;

	// TODO : 1.Abilities_Attack을 제외한 모든 Attack태그는 제거할 예정이므로, 
	//			AttackAbility를 수정하였다면 여기서 제거할 것.
	//		  2.이제 태그는 기존에 Basic이라고 불렀던 일반 태그만 존재하므로, 
	//			Unique태그는 UniqueAbility를 제거 혹은 수정하였다면 여기서 제거할 것.
	//		  3.이제 무기 스킬이 아닌 일반 스킬이므로, 이전처럼 무기 이름으로 
	//			태그를 만드는 것이 아니라, 스킬 이름으로 태그 이름을 지정할 것.

	FGameplayTag Abilities_Explosion; // 이런식으로 하나만 남기면 됨. (폭발 마법)

	FGameplayTag Abilities_FIreStaff_Attack;			FGameplayTag Abilities_FIreStaff_Basic;				FGameplayTag Abilities_FIreStaff_Unique;
	FGameplayTag Abilities_GravityStaff_Attack;			FGameplayTag Abilities_GravityStaff_Basic;			FGameplayTag Abilities_GravityStaff_Unique;
	FGameplayTag Abilities_LightStaff_Attack;			FGameplayTag Abilities_LightStaff_Basic;			FGameplayTag Abilities_LightStaff_Unique;
	FGameplayTag Abilities_MagicShield_Attack;			FGameplayTag Abilities_MagicShield_Basic;			FGameplayTag Abilities_MagicShield_Unique;
	FGameplayTag Abilities_HugeShield_Attack;			FGameplayTag Abilities_HugeShield_Basic;			FGameplayTag Abilities_HugeShield_Unique;
	FGameplayTag Abilities_Rappier_Attack;				FGameplayTag Abilities_Rappier_Basic;				FGameplayTag Abilities_Rappier_Unique;
	FGameplayTag Abilities_ShortBow_Attack;				FGameplayTag Abilities_ShortBow_Basic;				FGameplayTag Abilities_ShortBow_Unique;
	FGameplayTag Abilities_Revolver_Attack;				FGameplayTag Abilities_Revolver_Basic;				FGameplayTag Abilities_Revolver_Unique;
	FGameplayTag Abilities_SkullStaff_Attack;			FGameplayTag Abilities_SkullStaff_Basic;			FGameplayTag Abilities_SkullStaff_Unique;
	FGameplayTag Abilities_Ninja_Attack;				FGameplayTag Abilities_Ninja_Basic;					FGameplayTag Abilities_Ninja_Unique;
	FGameplayTag Abilities_WorldTreesBranch_Attack;		FGameplayTag Abilities_WorldTreesBranch_Basic;		FGameplayTag Abilities_WorldTreesBranch_Unique;
	FGameplayTag Abilities_RedFlag_Attack;				FGameplayTag Abilities_RedFlag_Basic;				FGameplayTag Abilities_RedFlag_Unique;
	FGameplayTag Abilities_LongBow_Attack;				FGameplayTag Abilities_LongBow_Basic;				FGameplayTag Abilities_LongBow_Unique;
	FGameplayTag Abilities_Hammer_Attack;				FGameplayTag Abilities_Hammer_Basic;				FGameplayTag Abilities_Hammer_Unique;
	FGameplayTag Abilities_Assassin_Attack;				FGameplayTag Abilities_Assassin_Basic;				FGameplayTag Abilities_Assassin_Unique;
	FGameplayTag Abilities_SledgeHammer_Attack;			FGameplayTag Abilities_SledgeHammer_Basic;			FGameplayTag Abilities_SledgeHammer_Unique;
	/** end Offensive Skill Ability Tags **/

	/** Offensive Skill Cooldown Tags **/

	// TODO :	여기도 마찬가지로 Unique와 Attack 제거. 
	// 
	// TODO ?	근데 쿨다운도 생각해보면 이펙트 하나만 사용하고 코드로 변수 하나 생성해서 
	//			블루프린트 내에서 계수 입력 -> 받아와 이펙트 쿨다운 값에 곱하기 + 태그 추가하는 방식으로 
	//			이펙트 하나로 운용할 수도 있을 것 같름 (아무튼간에 Attack태그는 필요가 없기에 제거해도 무방함.)

	FGameplayTag Cooldown_Explosion; // 이렇게 심플하게 하나만.

	FGameplayTag Cooldown_FIreStaff_Attack;				FGameplayTag Cooldown_FIreStaff_Basic;				FGameplayTag Cooldown_FIreStaff_Unique;
	FGameplayTag Cooldown_GravityStaff_Attack;			FGameplayTag Cooldown_GravityStaff_Basic;			FGameplayTag Cooldown_GravityStaff_Unique;
	FGameplayTag Cooldown_LightStaff_Attack;			FGameplayTag Cooldown_LightStaff_Basic;				FGameplayTag Cooldown_LightStaff_Unique;
	FGameplayTag Cooldown_MagicShield_Attack;			FGameplayTag Cooldown_MagicShield_Basic;			FGameplayTag Cooldown_MagicShield_Unique;
	FGameplayTag Cooldown_HugeShield_Attack;			FGameplayTag Cooldown_HugeShield_Basic;				FGameplayTag Cooldown_HugeShield_Unique;
	FGameplayTag Cooldown_Rappier_Attack;				FGameplayTag Cooldown_Rappier_Basic;				FGameplayTag Cooldown_Rappier_Unique;
	FGameplayTag Cooldown_ShortBow_Attack;				FGameplayTag Cooldown_ShortBow_Basic;				FGameplayTag Cooldown_ShortBow_Unique;
	FGameplayTag Cooldown_Revolver_Attack;				FGameplayTag Cooldown_Revolver_Basic;				FGameplayTag Cooldown_Revolver_Unique;
	FGameplayTag Cooldown_SkullStaff_Attack;			FGameplayTag Cooldown_SkullStaff_Basic;				FGameplayTag Cooldown_SkullStaff_Unique;
	FGameplayTag Cooldown_Ninja_Attack;					FGameplayTag Cooldown_Ninja_Basic;					FGameplayTag Cooldown_Ninja_Unique;
	FGameplayTag Cooldown_WorldTreesBranch_Attack;		FGameplayTag Cooldown_WorldTreesBranch_Basic;		FGameplayTag Cooldown_WorldTreesBranch_Unique;
	FGameplayTag Cooldown_RedFlag_Attack;				FGameplayTag Cooldown_RedFlag_Basic;				FGameplayTag Cooldown_RedFlag_Unique;
	FGameplayTag Cooldown_LongBow_Attack;				FGameplayTag Cooldown_LongBow_Basic;				FGameplayTag Cooldown_LongBow_Unique;
	FGameplayTag Cooldown_Hammer_Attack;				FGameplayTag Cooldown_Hammer_Basic;					FGameplayTag Cooldown_Hammer_Unique;
	FGameplayTag Cooldown_Assassin_Attack;				FGameplayTag Cooldown_Assassin_Basic;				FGameplayTag Cooldown_Assassin_Unique;
	FGameplayTag Cooldown_SledgeHammer_Attack;			FGameplayTag Cooldown_SledgeHammer_Basic;			FGameplayTag Cooldown_SledgeHammer_Unique;
	/** end Offensive Skill Cooldown Tags **/

	/** Passive Skill Ability Tags **/
	FGameplayTag Abilities_Passive_ManaArrow;
	/** end Passive Skill Ability Tags **/

	/** Scroll Skill Ability Tags **/
	FGameplayTag Abilities_Scroll_Blink;
	/** end Scroll Skill Ability Tags **/

	/** Scroll Skill Cooldown Tags **/
	FGameplayTag Cooldown_Scroll_Blink;
	/** end Scroll Skill Cooldown Tags **/



	/** Weapon Tags **/
	FGameplayTag Weapon_FireStaff;	// 스태프는 양손 마법 지팡이,
	FGameplayTag Weapon_VoidWand;	// 완드는 한손 마법 봉 (해리포터 마법지팡이)
	FGameplayTag Weapon_DoubleHammer;
	FGameplayTag Weapon_Broadsword;
	FGameplayTag Weapon_Longbow;	// 대문자는 무조건 붙이지X 번역기 돌려보고 하나의 단어로 존재하면 붙여서 선언.
	/** end Weapon Tags **/



	FGameplayTag CombatSocket_ActorLocation;
	FGameplayTag CombatSocket_Weapon;
	FGameplayTag CombatSocket_RightHand;
	FGameplayTag CombatSocket_LeftHand;

	FGameplayTag Montage_Attack_1;
	FGameplayTag Montage_Attack_2;
	FGameplayTag Montage_Attack_3;
	FGameplayTag Montage_Attack_4;

	TArray<FGameplayTag> DamageTypes;
	TArray<FGameplayTag> DamageCoeffAttributes;
	TArray<FGameplayTag> Runes;
	TMap<FGameplayTag, FResistanceInfo> DamageTypesToResistanceInfos;
	TMap<FGameplayTag, FDebuffInfo> DebuffTagsToDebuffInfos;

	FGameplayTag Effects_HitReact;

	FGameplayTag Player_Block_InputPressed;
	FGameplayTag Player_Block_InputHeld;
	FGameplayTag Player_Block_InputReleased;
	FGameplayTag Player_Block_CursorTrace;

private:
	static FCubeGameplayTags GameplayTags;
};


