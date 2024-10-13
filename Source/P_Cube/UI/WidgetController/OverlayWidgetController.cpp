// Fill out your copyright notice in the Description page of Project Settings.


#include "P_Cube/UI/WidgetController/OverlayWidgetController.h"

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
	else if (AbilityStatus.MatchesTagExact(GameplayTags.Abilities_Status_Unlocked))
	{
		if (SkillPoints > 0)
		{
			bShouldAddUniqueButton = true;
		}
	}
}

void UOverlayWidgetController::SpendSkillPointButtonPressed()
{
	if (GetCubeASC())
	{
		GetCubeASC()->ServerSpendSkillPoint(SelectedUniqueAbility.UniqueAbility);
	}
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
		AbilityStatus = GameplayTags.Abilities_Status_Locked;
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
