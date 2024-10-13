// Fill out your copyright notice in the Description page of Project Settings.


#include "CubeAbilitySystemComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "P_Cube/CubeGameplayTags.h"
#include "P_Cube/AbilitySystem/CubeAbilitySystemLibrary.h"
#include "P_Cube/AbilitySystem/Abilities/CubeGameplayAbility.h"
#include "P_Cube/AbilitySystem/Data/AbilityInfo.h"
#include "P_Cube/CubeLogChannels.h"
#include "P_Cube/Interaction/PlayerInterface.h"

#include "GameplayTagsManager.h"

void UCubeAbilitySystemComponent::AbilityActorInfoSet()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UCubeAbilitySystemComponent::ClientEffectApplied);
}

void UCubeAbilitySystemComponent::AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities)
{
	for (const TSubclassOf<UGameplayAbility> AbilityClass : StartupAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		if (const UCubeGameplayAbility* CubeAbility = Cast<UCubeGameplayAbility>(AbilitySpec.Ability))
		{
			AbilitySpec.DynamicAbilityTags.AddTag(CubeAbility->StartupInputTag);
			AbilitySpec.DynamicAbilityTags.AddTag(FCubeGameplayTags::Get().Abilities_Status_Unlocked);
			GiveAbility(AbilitySpec);
		}
	}
	bStartupAbilitiesGiven = true;
	AbilitiesGivenDelegate.Broadcast();
}

void UCubeAbilitySystemComponent::AddCharacterPassiveAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupPassiveAbilities)
{
	for (const TSubclassOf<UGameplayAbility> AbilityClass : StartupPassiveAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		GiveAbilityAndActivateOnce(AbilitySpec);
	}
}

void UCubeAbilitySystemComponent::AbilityInputTagHeld(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;

	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			AbilitySpecInputPressed(AbilitySpec);
			if (!AbilitySpec.IsActive())
			{
				TryActivateAbility(AbilitySpec.Handle);
			}
		}
	}
}

void UCubeAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;

	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			AbilitySpecInputReleased(AbilitySpec);
		}
	}
}

void UCubeAbilitySystemComponent::ForEachAbility(const FForEachAbility& Delegate)
{
	FScopedAbilityListLock ActiveScopeLock(*this);
	for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (!Delegate.ExecuteIfBound(AbilitySpec))
		{
			UE_LOG(LogCube, Error, TEXT("Failed to execute delegate in %hs"), __FUNCTION__);
		}
	}
}

FGameplayTag UCubeAbilitySystemComponent::GetAbilityTagFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	if (AbilitySpec.Ability)
	{
		for (FGameplayTag Tag : AbilitySpec.Ability.Get()->AbilityTags)
		{
			if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Abilities"))))
			{
				return Tag;
			}
		}
	}
	return FGameplayTag();
}

FGameplayTag UCubeAbilitySystemComponent::GetInputTagFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	for (FGameplayTag Tag : AbilitySpec.DynamicAbilityTags)
	{
		if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("InputTag"))))
		{
			return Tag;
		}
	}
	return FGameplayTag();
}

FGameplayTag UCubeAbilitySystemComponent::GetStatusFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	for (FGameplayTag StatusTag : AbilitySpec.DynamicAbilityTags)
	{
		if (StatusTag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Abilities.Status"))))
		{
			return StatusTag;
		}
	}
	return FGameplayTag();
}

FGameplayAbilitySpec* UCubeAbilitySystemComponent::GetSpecFromAbilityTag(const FGameplayTag& AbilityTag)
{
	FScopedAbilityListLock ActiveScopeLoc(*this);
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		for (FGameplayTag Tag : AbilitySpec.Ability.Get()->AbilityTags)
		{
			if (Tag.MatchesTag(AbilityTag))
			{
				return &AbilitySpec;
			}
		}
	}
	return nullptr;
}

void UCubeAbilitySystemComponent::UpgradeAttribute(const FGameplayTag& AttributeTag)
{
	if (GetAvatarActor()->Implements<UPlayerInterface>())
	{
		if (IPlayerInterface::Execute_GetMoney(GetAvatarActor()) > 0)
		{
			//ServerUpgradeAttribute(AttributeTag);
		}
	}
}

void UCubeAbilitySystemComponent::ServerUpgradeAttribute_Implementation(const FGameplayTag& AttributeTag)
{
	FGameplayEventData Payload;
	Payload.EventTag = AttributeTag;
	Payload.EventMagnitude = 1.f;
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetAvatarActor(), AttributeTag, Payload);
	if (GetAvatarActor()->Implements<UPlayerInterface>())
	{
		IPlayerInterface::Execute_AddToMoney(GetAvatarActor(), -1);
	}
}

void UCubeAbilitySystemComponent::UpdateAbilityStatuses(int32 Level)
{
	UAbilityInfo* AbilityInfo = UCubeAbilitySystemLibrary::GetAbilityInfo(GetAvatarActor());
	for (const FCubeAbilityInfo& Info : AbilityInfo->AbilityInformation)
	{
		if (!Info.AbilityTag.IsValid()) continue;
		if (Level < Info.LevelRequirement) continue;
		if (GetSpecFromAbilityTag(Info.AbilityTag) == nullptr)
		{
			FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(Info.Ability, 1);
			AbilitySpec.DynamicAbilityTags.AddTag(FCubeGameplayTags::Get().Abilities_Status_Unlocked);
			GiveAbility(AbilitySpec);
			MarkAbilitySpecDirty(AbilitySpec);
			ClientUpdateAbilityStatus(Info.AbilityTag, FCubeGameplayTags::Get().Abilities_Status_Unlocked, Level); // ��ų ������ ������ ����.
		}
	}
}

void UCubeAbilitySystemComponent::ServerSpendSkillPoint_Implementation(const FGameplayTag& AbilityTag)
{
	/*if (FGameplayAbilitySpec* AbilitySpec = GetSpecFromAbilityTag(AbilityTag))
	{
		if (GetAvatarActor()->Implements<UPlayerInterface>())
		{
			IPlayerInterface::Execute_AddToSkillPoints(GetAvatarActor(), -1);
		}

		const FCubeGameplayTags GameplayTags = FCubeGameplayTags::Get();
		FGameplayTag Status = GetStatusFromSpec(*AbilitySpec);
		if (Status.MatchesTagExact(GameplayTags.Abilities_Status_Unlocked))
		{
			AbilitySpec->Level += 1;
		}
		ClientUpdateAbilityStatus(AbilityTag, Status, AbilitySpec->Level);
		MarkAbilitySpecDirty(*AbilitySpec);
	}*/ // -> ��ų �������ϴ� �Լ����µ�, ���� �ʿ� ����. �츮 ��ų�� �÷��̾� ������ ����ȭ�Ǳ� ������. �̰� ���߿� ����ũ��ų ���׷��̵�� ����� ��.
}

bool UCubeAbilitySystemComponent::GetDescriptionsByAbilityTag(const FGameplayTag& AbilityTag, FString& OutBasicDescription, FString& OutUniqueDescription, bool bIsDetail)
{
	if (const FGameplayAbilitySpec* BasicAbilitySpec = GetSpecFromAbilityTag(AbilityTag))
	{
		if (UCubeGameplayAbility* CubeAbility = Cast<UCubeGameplayAbility>(BasicAbilitySpec->Ability))
		{
			bIsDetail ? 
				OutBasicDescription = CubeAbility->GetDetailedBasicDescription(BasicAbilitySpec->Level)
				:
				OutBasicDescription = CubeAbility->GetBasicDescription(BasicAbilitySpec->Level);

			if (const FGameplayAbilitySpec* UniqueAbilitySpec = GetSpecFromAbilityTag(GetUniqueTagFromBasicTag(AbilityTag)))
			{
				bIsDetail ?
					OutUniqueDescription = CubeAbility->GetDetailedUniqueDescription(UniqueAbilitySpec->Level)
					:
					OutUniqueDescription = CubeAbility->GetUniqueDescription(UniqueAbilitySpec->Level);
			}

			return true;
		}
	}

	OutBasicDescription = FString();
	OutUniqueDescription = FString();

	return false;
}

FGameplayTag UCubeAbilitySystemComponent::GetUniqueTagFromBasicTag(const FGameplayTag& BasicTag)
{
	// �±� ���ڿ��� ��ȯ
	FString TagString = BasicTag.ToString();

	// .Basic���� �������� Ȯ��
	if (TagString.EndsWith(TEXT(".Basic")))
	{
		// .Basic�� .Unique�� ����
		TagString.RemoveFromEnd(TEXT(".Basic"));
		TagString.Append(TEXT(".Unique"));

		return UGameplayTagsManager::Get().RequestGameplayTag(FName(*TagString));
	}

	// .Basic�� �ƴ� ���, �⺻ �±׸� �״�� ��ȯ
	return FGameplayTag();
}

void UCubeAbilitySystemComponent::OnRep_ActivateAbilities()
{
	Super::OnRep_ActivateAbilities();

	if (!bStartupAbilitiesGiven)
	{
		bStartupAbilitiesGiven = true;
		AbilitiesGivenDelegate.Broadcast();
	}
}

void UCubeAbilitySystemComponent::ClientUpdateAbilityStatus_Implementation(const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, int32 AbilityLevel)
{
	AbilityStatusChanged.Broadcast(AbilityTag, StatusTag, AbilityLevel);
}

void UCubeAbilitySystemComponent::ClientEffectApplied_Implementation(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle)
{
	FGameplayTagContainer TagContainer;
	EffectSpec.GetAllAssetTags(TagContainer);

	EffectAssetTags.Broadcast(TagContainer); // ���� �±׸� ������ �۵��ϴ� ��ε�ĳ��Ʈ.
}
