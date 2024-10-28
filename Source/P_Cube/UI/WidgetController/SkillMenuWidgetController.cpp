// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillMenuWidgetController.h"

#include "P_Cube/AbilitySystem/CubeAbilitySystemComponent.h"
#include "P_Cube/AbilitySystem/Data/AbilityInfo.h"
#include "P_Cube/Player/CubePlayerState.h"

void USkillMenuWidgetController::BroadcastInitialValues()
{
	BroadcastAbilityInfo();
	SkillPointsChanged.Broadcast(GetCubePS()->GetSkillPoints());
}

void USkillMenuWidgetController::BindCallbacksToDependencies()
{
	GetCubeASC()->AbilityStatusChanged.AddLambda([this](const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, int32 NewLevel)
		{
			//GetCubeASC()->AbilityEquipped.AddUObject(this, &USkillMenuWidgetController::OnAbilityEquipped);
			if (SelectedAbility.Ability.MatchesTagExact(AbilityTag))
			{
				SelectedAbility.Status = StatusTag;
				bool bEnableSpendPoints = false;
				bool bEnableEquip = false;

				FString BasicDescription;
				FString UniqueDescription;
				GetCubeASC()->GetDescriptionsByAbilityTag(AbilityTag, BasicDescription, UniqueDescription, true);
				SkillMenuBoxSelectedDelegate.Broadcast(BasicDescription, UniqueDescription);
			}
			
			if (AbilityInfo)
			{
				FCubeAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);
				Info.StatusTag = StatusTag;
				AbilityInfoDelegate.Broadcast(Info);
			}
		});

	GetCubePS()->OnSkillPointsChangedDelegate.AddLambda([this](int32 SkillPoints)
		{
			SkillPointsChanged.Broadcast(SkillPoints);

			FString BasicDescription;
			FString UniqueDescription;
			GetCubeASC()->GetDescriptionsByAbilityTag(SelectedAbility.Ability, BasicDescription, UniqueDescription, true);
			SkillMenuBoxSelectedDelegate.Broadcast(BasicDescription, UniqueDescription);
		});
}

void USkillMenuWidgetController::SkillBoxSelected(const FGameplayTag& AbilityTag)
{
	const FCubeGameplayTags GameplayTags = FCubeGameplayTags::Get();
	const int32 SkillPoints = GetCubePS()->GetSkillPoints();
	FGameplayTag AbilityStatus;

	const bool bTagValid = AbilityTag.IsValid();
	const bool bTagNone = AbilityTag.MatchesTag(GameplayTags.Abilities_None);
	const FGameplayAbilitySpec* AbilitySpec = GetCubeASC()->GetSpecFromAbilityTag(AbilityTag);
	const bool bSpecValid = AbilitySpec != nullptr;
	if (!bTagValid || bTagNone || !bSpecValid)
	{
		AbilityStatus = GameplayTags.Abilities_Status_UnEquipped; // 원래 Locked였는데 무슨 쓸모인지 잘 모르겠음
	}
	else
	{
		AbilityStatus = GetCubeASC()->GetStatusFromSpec(*AbilitySpec);
	}
	bool bEnableSpendPoints = false;
	bool bEnableEquip = false;

	FString BasicDescription;
	FString UniqueDescription;
	GetCubeASC()->GetDescriptionsByAbilityTag(AbilityTag, BasicDescription, UniqueDescription, true);
	SkillMenuBoxSelectedDelegate.Broadcast(BasicDescription, UniqueDescription);
}

void USkillMenuWidgetController::BoxDeselect()
{
	SelectedAbility.Ability = FCubeGameplayTags::Get().Abilities_None;
	SelectedAbility.Status = FCubeGameplayTags::Get().Abilities_Status_UnEquipped;
	SkillMenuBoxSelectedDelegate.Broadcast(FString(), FString());
}

//void USkillMenuWidgetController::OnAbilityEquipped(const FGameplayTag& AbilityTag, const FGameplayTag& Status, const FGameplayTag& Slot, const FGameplayTag& PreviousSlot)
//{
//	const FCubeGameplayTags& GameplayTags = FCubeGameplayTags::Get();
//
//	FCubeAbilityInfo LastSlotInfo;
//	LastSlotInfo.StatusTag = GameplayTags.Abilities_Status_UnEquipped;
//	LastSlotInfo.InputTag = PreviousSlot;
//	LastSlotInfo.AbilityTag = GameplayTags.Abilities_None;
//	// Broadcast empty info if PreviousSlot is a valid slot. Only if equipping an already-equipped spell
//	AbilityInfoDelegate.Broadcast(LastSlotInfo);
//	FCubeAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);
//	Info.StatusTag = Status;
//	Info.InputTag = Slot;
//	AbilityInfoDelegate.Broadcast(Info);
//
//	SkillBoxReassignedDelegate.Broadcast(AbilityTag);
//	BoxDeselect();
//}
