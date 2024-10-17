// Fill out your copyright notice in the Description page of Project Settings.


#include "P_Cube/UI/WidgetController/OverlayWidgetController.h"

#include "P_Cube/CubeGameplayTags.h"
#include "P_Cube/AbilitySystem/CubeAbilitySystemComponent.h"
#include "P_Cube/AbilitySystem/CubeAttributeSet.h"
#include "P_Cube/AbilitySystem/Data/AbilityInfo.h"
#include "P_Cube/AbilitySystem/Data/LevelUpInfo.h"
#include "P_Cube/Player/CubePlayerState.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	OnHealthChanged.Broadcast(GetCubeAS()->GetHealth()); // 체력 값이 변경되면 브로드캐스팅 한다. (OnHealthChanged_Event 블루프린트가 트리거 됨.)
	OnMaxHealthChanged.Broadcast(GetCubeAS()->GetMaxHealth()); // 마찬가지로 최대체력
	OnManaChanged.Broadcast(GetCubeAS()->GetMana()); // 마나
	OnMaxManaChanged.Broadcast(GetCubeAS()->GetMaxMana()); // 최대마나
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	GetCubePS()->OnXPChangedDelegate.AddUObject(this, &UOverlayWidgetController::OnXPChanged);
	GetCubePS()->OnLevelChangedDelegate.AddLambda(
		[this](int32 NewLevel)
		{
			OnPlayerLevelChangedDelegate.Broadcast(NewLevel);
		}
	);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetCubeAS()->GetHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnHealthChanged.Broadcast(Data.NewValue);
		}
	);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetCubeAS()->GetMaxHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnMaxHealthChanged.Broadcast(Data.NewValue);
		}
	);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetCubeAS()->GetManaAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnManaChanged.Broadcast(Data.NewValue);
		}
	);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetCubeAS()->GetMaxManaAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnMaxManaChanged.Broadcast(Data.NewValue);
		}
	);

	if (GetCubeASC())
	{
		GetCubeASC()->AbilityEquipped.AddUObject(this, &UOverlayWidgetController::OnAbilityEquipped);
		if (GetCubeASC()->bStartupAbilitiesGiven)
		{
			BroadcastAbilityInfo();
		}
		else
		{
			GetCubeASC()->AbilitiesGivenDelegate.AddUObject(this, &UOverlayWidgetController::BroadcastAbilityInfo);
		}

		GetCubeASC()->EffectAssetTags.AddLambda(
			[this](const FGameplayTagContainer& AssetTags)
			{
				for (const FGameplayTag& Tag : AssetTags)
				{
					// For example, say that Tag = Message.HealthPotion
					// "Message.HealthPotion".MatchesTag("Message") will return True, "Message".MatchesTag("Message.HealthPotion") will return False
					// Message 상위 태그를 포함하는 Asset태그만 if문을 통과함.
					FGameplayTag MessageTag = FGameplayTag::RequestGameplayTag(FName("Message"));
					if (Tag.MatchesTag(MessageTag))
					{
						const FUIWidgetRow* Row = GetDataTableRowByTag<FUIWidgetRow>(MessageWidgetDataTable, Tag);
						MessageWidgetRowDelegate.Broadcast(*Row);
					}
				}
			}
		);
	}

	GetCubeASC()->AbilityStatusChanged.AddLambda([this](const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, int32 NewLevel)
		{
			if (SelectedUniqueAbility.UniqueAbility.MatchesTagExact(AbilityTag))
			{
				SelectedUniqueAbility.Status = StatusTag;
				bool bEnableSpendPoints = false;
				bool bShouldEnableUniqueButton = false;
				ShouldEnableUniqueButton(StatusTag, CurrentSkillPoints, bShouldEnableUniqueButton);
				EnableUniqueSkillDelegate.Broadcast(bShouldEnableUniqueButton);
			}

			if (AbilityInfo)
			{
				FCubeAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);
				Info.StatusTag = StatusTag;
				AbilityInfoDelegate.Broadcast(Info);
			}
		});

	GetCubeASC()->AbilityEquipped.AddUObject(this, &UOverlayWidgetController::OnAbilityEquipped);

	GetCubePS()->OnSkillPointsChangedDelegate.AddLambda([this](int32 SkillPoints)
		{
			SkillPointsChanged.Broadcast(SkillPoints);
			CurrentSkillPoints = SkillPoints;
			bool bShouldEnableUniqueButton = false;
			ShouldEnableUniqueButton(SelectedUniqueAbility.Status, CurrentSkillPoints, bShouldEnableUniqueButton);
			EnableUniqueSkillDelegate.Broadcast(bShouldEnableUniqueButton);
		});
}

void UOverlayWidgetController::OnXPChanged(int32 NewXP)
{
	const ULevelUpInfo* LevelUpInfo = GetCubePS()->LevelUpInfo;
	checkf(LevelUpInfo, TEXT("Unabled to find LevelUpInfo. Please fill out CubePlayerState Blueprint"));

	const int32 Level = LevelUpInfo->FindLevelForXP(NewXP);
	const int32 MaxLevel = LevelUpInfo->LevelUpInformation.Num();

	if (Level <= MaxLevel && Level > 0)
	{
		const int32 LevelUpRequirement = LevelUpInfo->LevelUpInformation[Level].LevelUpRequirement;
		const int32 PreviousLevelUpRequirement = LevelUpInfo->LevelUpInformation[Level - 1].LevelUpRequirement;

		const int32 DeltaLevelRequirement = LevelUpRequirement - PreviousLevelUpRequirement;
		const int32 XPForThisLevel = NewXP - PreviousLevelUpRequirement;

		const float XPBarPercent = static_cast<float>(XPForThisLevel) / static_cast<float>(DeltaLevelRequirement);

		OnXPPercentChangedDelegate.Broadcast(XPBarPercent);
	}
}

void UOverlayWidgetController::ShouldEnableUniqueButton(const FGameplayTag& AbilityStatus, int32 SkillPoints, bool& bShouldAddUniqueButton)
{
	const FCubeGameplayTags GameplayTags = FCubeGameplayTags::Get();
	bShouldAddUniqueButton = false;
	if (AbilityStatus.MatchesTagExact(GameplayTags.Abilities_Status_Fixed))
	{
		if (SkillPoints > 0)
		{
			bShouldAddUniqueButton = true;
		}
	}
	else if (AbilityStatus.MatchesTagExact(GameplayTags.Abilities_Status_UnEquipped))
	{
		if (SkillPoints > 0)
		{
			bShouldAddUniqueButton = true;
		}
	}
}

void UOverlayWidgetController::SpendSkillPointButtonPressed(const FGameplayTag& UniqueAbilityTag, const FGameplayTag& SlotTag)
{
	GetCubeASC()->ServerEquipAbility(UniqueAbilityTag, SlotTag, true);

	if (GetCubeASC())
	{
		//GetCubeASC()->ServerSpendSkillPoint(SelectedUniqueAbility.UniqueAbility);

		//GetCubeASC()->ServerEquipAbility(SelectedUniqueAbility.UniqueAbility, SlotTag);
	}
}

void UOverlayWidgetController::EquipSkillBoxSelected(const FGameplayTag& AbilityTag)
{
	if ( bWaitingForEquipSelection )
	{
		const FGameplayTag SelectedAbilityType = AbilityInfo->FindAbilityInfoForTag(AbilityTag).AbilityType;
		StopWaitingForEquipDelegate.Broadcast(SelectedAbilityType);
		bWaitingForEquipSelection = false;
	}
}

void UOverlayWidgetController::WeaponSkillEquipButtonPressed(const FGameplayTag& AbilityTypeTag, const FGameplayTag& AbilityTag) // 스킬 장비하겠다고 알리는 함수.
{
	WaitForEquipDelegate.Broadcast(AbilityTypeTag); // 스킬 장비 버튼 활성화 델리게이트 call (TypeTag로 스크롤 스킬인지 아닌지 구분.)
	bWaitingForEquipSelection = true; // 장비 가능 상태로 전환

	SelectedWeaponAbility = AbilityTag; // 장비할 스킬을 변수에 담기.
}

void UOverlayWidgetController::SwapSkillSlot(const FGameplayTag& AbilityTag)
{
	SelectedSwapWeaponAbility = AbilityTag;

	const FGameplayTag SelectedStatus = GetCubeASC()->GetStatusFromAbilityTag(SelectedSwapWeaponAbility);
	if ( SelectedStatus.MatchesTagExact(FCubeGameplayTags::Get().Abilities_Status_Equipped) ) // 만약 이미 가지고 있는 스킬이라면?
	{
		SelectedSwapSlot = GetCubeASC()->GetInputTagFromAbilityTag(SelectedWeaponAbility); // 옮길 스킬의 슬롯 찾기
	}

	// TODO : 스킬 슬롯과 스킬을 알고 있으니, 옮길 슬롯 위치와 슬롯 위치의 스킬을 받아와 서로 스왑하기.
}

void UOverlayWidgetController::EquipSkillBoxPressed(const FGameplayTag& SlotTag, const FGameplayTag& AbilityType)
{
	if ( !bWaitingForEquipSelection ) return; // 장비 상태가 아니면 리턴.
	// Check selected ability against the slot's ability type.
	// (don't equip an offensive spell in a passive slot and vice versa)
	const FGameplayTag& SelectedAbilityType = AbilityInfo->FindAbilityInfoForTag(SelectedWeaponAbility).AbilityType; // 선택한 스킬의 타입을 저장.
	if ( !SelectedAbilityType.MatchesTagExact(AbilityType) ) return; // 스킬 타입이 스크롤인지 일반인지 다시 한번 체크하고, 다르다면 리턴.

	GetCubeASC()->ServerEquipAbility(SelectedWeaponAbility, SlotTag, false); // 서버 단위로 작동하는 스킬 장비 함수를 호출.
}

FGameplayTag UOverlayWidgetController::GetUniqueTagByBasicTag(const FGameplayTag& BasicTag)
{
	return AbilityInfo->FindUniqueTagForBasicTag(BasicTag);
}

FCubeAbilityInfo UOverlayWidgetController::GetAbilityInfoByTag(const FGameplayTag& Tag)
{
	return AbilityInfo->FindAbilityInfoForTag(Tag);
}

void UOverlayWidgetController::OnAbilityEquipped(const FGameplayTag & AbilityTag, const FGameplayTag & Status, const FGameplayTag & Slot, const FGameplayTag & PreviousSlot)
{
	bWaitingForEquipSelection = false;
	const FCubeGameplayTags& GameplayTags = FCubeGameplayTags::Get();

	FCubeAbilityInfo LastSlotInfo;
	LastSlotInfo.StatusTag = GameplayTags.Abilities_Status_UnEquipped;
	LastSlotInfo.InputTag = PreviousSlot;
	LastSlotInfo.AbilityTag = GameplayTags.Abilities_None;
	// Broadcast empty info if PreviousSlot is a valid slot. Only if equipping an already-equipped spell
	AbilityInfoDelegate.Broadcast(LastSlotInfo);
	FCubeAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);
	FCubeAbilityInfo UniqueInfo = AbilityInfo->FindAbilityInfoForTag(AbilityInfo->FindUniqueTagForBasicTag(AbilityTag));
	Info.StatusTag = Status;
	Info.InputTag = Slot;
	AbilityInfoDelegate.Broadcast(Info);
	AbilityInfoDelegate.Broadcast(UniqueInfo);
	StopWaitingForEquipDelegate.Broadcast(AbilityInfo->FindAbilityInfoForTag(AbilityTag).AbilityType);
}

void UOverlayWidgetController::UpdateUniqueSkillEnable(const FGameplayTag& AbilityTag)
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
		AbilityStatus = GameplayTags.Abilities_Status_Equipped;
	}
	else
	{
		AbilityStatus = GetCubeASC()->GetStatusFromSpec(*AbilitySpec);
	}

	SelectedUniqueAbility.UniqueAbility = AbilityTag;
	SelectedUniqueAbility.Status = AbilityStatus;
	bool bShouldAddUniqueButton = false;
	ShouldEnableUniqueButton(AbilityStatus, SkillPoints, bShouldAddUniqueButton);
	EnableUniqueSkillDelegate.Broadcast(bShouldAddUniqueButton);
}
