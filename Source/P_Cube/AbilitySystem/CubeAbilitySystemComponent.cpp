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

			if ( CubeAbility->StartupInputTag == FCubeGameplayTags::Get().InputTag_Q ) // Q스킬이면 고정.
			{
				AbilitySpec.DynamicAbilityTags.AddTag(FCubeGameplayTags::Get().Abilities_Status_Fixed);
			}
			else
			{
				AbilitySpec.DynamicAbilityTags.AddTag(FCubeGameplayTags::Get().Abilities_Status_UnEquipped);
			}
			
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

FGameplayTag UCubeAbilitySystemComponent::GetStatusFromAbilityTag(const FGameplayTag& AbilityTag)
{
	if ( const FGameplayAbilitySpec* Spec = GetSpecFromAbilityTag(AbilityTag) )
	{
		return GetStatusFromSpec(*Spec);
	}
	return FGameplayTag();
}

FGameplayTag UCubeAbilitySystemComponent::GetInputTagFromAbilityTag(const FGameplayTag& AbilityTag)
{
	if ( const FGameplayAbilitySpec* Spec = GetSpecFromAbilityTag(AbilityTag) )
	{
		return GetInputTagFromSpec(*Spec);
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
	UAbilityInfo* AbilityInfo = UCubeAbilitySystemLibrary::GetAbilityInfo(GetAvatarActor()); // 캐릭터가 가진 스킬정보를 받아옴
	for (const FCubeAbilityInfo& Info : AbilityInfo->AbilityInformation) // 스킬 정보들을 foreach함.
	{
		if (!Info.AbilityTag.IsValid()) continue; // 어빌리티 태그도 없는 스킬이면 넘어감.
		if (Level < Info.LevelRequirement) continue; // 요구 레벨보다 낮으면 넘어감.
		if (GetSpecFromAbilityTag(Info.AbilityTag) == nullptr) // 어빌리티 태그로 스펙을 받아옴.
		{
			FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(Info.Ability, 1); 
			AbilitySpec.DynamicAbilityTags.AddTag(FCubeGameplayTags::Get().Abilities_Status_UnEquipped);
			GiveAbility(AbilitySpec);
			MarkAbilitySpecDirty(AbilitySpec);
			ClientUpdateAbilityStatus(Info.AbilityTag, FCubeGameplayTags::Get().Abilities_Status_UnEquipped, Level); // 스킬 레벨도 레벨로 설정.
		}
	}
}

void UCubeAbilitySystemComponent::ServerSpendSkillPoint_Implementation(const FGameplayTag& AbilityTag, const FGameplayTag& Slot)
{
	//if ( FGameplayAbilitySpec* AbilitySpec = GetSpecFromAbilityTag(AbilityTag) ) // 장비할 스킬의 스펙을 받아옴.
	//{
	//	const FCubeGameplayTags& GameplayTags = FCubeGameplayTags::Get();
	//	const FGameplayTag& PrevSlot = GetInputTagFromSpec(*AbilitySpec); // 해당 스킬의 인풋태그를 받아옴.
	//	const FGameplayTag& Status = GetStatusFromSpec(*AbilitySpec); // 해당 스킬의 상태를 받아옴. (장착중인지, 아닌지)



	//	const bool bStatusValid =  // 스킬이 이미 장착되었거나, 고정 상태이면 유효하지 않음.
	//		Status == GameplayTags.Abilities_Status_UnEquipped || Status == GameplayTags.Abilities_Status_Fixed;

	//	if ( bStatusValid )
	//	{
	//		// Remove this InputTag (slot) from any Ability that has it.
	//		ClearAbilitiesOfSlot(Slot);
	//		// Clear this ability's slot, just in case, it's a different slot
	//		ClearSlot(AbilitySpec);
	//		// Now, assign this ability to this slot
	//		AbilitySpec->DynamicAbilityTags.AddTag(Slot);

	//		if ( Status.MatchesTagExact(GameplayTags.Abilities_Status_UnEquipped) )
	//		{
	//			AbilitySpec->DynamicAbilityTags.RemoveTag(GameplayTags.Abilities_Status_UnEquipped);
	//			AbilitySpec->DynamicAbilityTags.AddTag(GameplayTags.Abilities_Status_Equipped);
	//		}
	//		MarkAbilitySpecDirty(*AbilitySpec);

	//		ClientEquipAbility(AbilityTag, GameplayTags.Abilities_Status_Equipped, Slot, PrevSlot);
	//	}

	ServerEquipAbility(AbilityTag, Slot, true);
}

void UCubeAbilitySystemComponent::ServerEquipAbility_Implementation(const FGameplayTag& AbilityTag, const FGameplayTag& Slot, const bool bAllowFixed)
{
	if ( FGameplayAbilitySpec* AbilitySpec = GetSpecFromAbilityTag(AbilityTag) ) // 장비할 스킬의 스펙을 받아옴.
	{
		const FCubeGameplayTags& GameplayTags = FCubeGameplayTags::Get();
		const FGameplayTag& PrevSlot = GetInputTagFromSpec(*AbilitySpec); // 해당 스킬의 인풋태그를 받아옴.
		const FGameplayTag& Status = GetStatusFromSpec(*AbilitySpec); // 해당 스킬의 상태를 받아옴. (장착중인지, 아닌지)



		bool bStatusValid =  // 스킬이 이미 장착되었거나, 고정 상태이면 유효하지 않음.
			Status == GameplayTags.Abilities_Status_UnEquipped;	

		if ( bAllowFixed && Status == GameplayTags.Abilities_Status_Fixed)
		{
			bStatusValid = true;
		}

		if ( bStatusValid )
		{
			// Remove this InputTag (slot) from any Ability that has it.
			ClearAbilitiesOfSlot(Slot);
			// Clear this ability's slot, just in case, it's a different slot
			ClearSlot(AbilitySpec);
			// Now, assign this ability to this slot
			AbilitySpec->DynamicAbilityTags.AddTag(Slot);

			if ( Status.MatchesTagExact(GameplayTags.Abilities_Status_UnEquipped) )
			{
				AbilitySpec->DynamicAbilityTags.RemoveTag(GameplayTags.Abilities_Status_UnEquipped);
				AbilitySpec->DynamicAbilityTags.AddTag(GameplayTags.Abilities_Status_Equipped);
			}
			MarkAbilitySpecDirty(*AbilitySpec);

			ClientEquipAbility(AbilityTag, GameplayTags.Abilities_Status_Equipped, Slot, PrevSlot);
		}

	}
}

void UCubeAbilitySystemComponent::ClientEquipAbility_Implementation(const FGameplayTag & AbilityTag, const FGameplayTag & Status, const FGameplayTag & Slot, const FGameplayTag & PreviousSlot)
{
	AbilityEquipped.Broadcast(AbilityTag, Status, Slot, PreviousSlot); // 클라이언트에 반영될 수 있도록 call
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

void UCubeAbilitySystemComponent::ClearSlot(FGameplayAbilitySpec* Spec)
{
	const FGameplayTag Slot = GetInputTagFromSpec(*Spec);
	Spec->DynamicAbilityTags.RemoveTag(Slot); // 슬롯을 비움
	MarkAbilitySpecDirty(*Spec); 
}

void UCubeAbilitySystemComponent::ClearAbilitiesOfSlot(const FGameplayTag & Slot)
{
	FScopedAbilityListLock ActiveScopeLock(*this);
	for ( FGameplayAbilitySpec& Spec : GetActivatableAbilities() )
	{
		if ( AbilityHasSlot(&Spec, Slot) ) // 스킬이 이미 슬롯을 가지고 있다면
		{
			ClearSlot(&Spec);
		}
	}
}

bool UCubeAbilitySystemComponent::AbilityHasSlot(FGameplayAbilitySpec * Spec, const FGameplayTag & Slot) // 스킬이 InputTag를 가지고 있는지를 반환하는 함수
{
	for ( FGameplayTag Tag : Spec->DynamicAbilityTags )
	{
		if ( Tag.MatchesTagExact(Slot) )
		{
			return true;
		}
	}
	return false;
}

FGameplayTag UCubeAbilitySystemComponent::GetUniqueTagFromBasicTag(const FGameplayTag& BasicTag)
{
	// 태그 문자열로 변환
	FString TagString = BasicTag.ToString();

	// .Basic으로 끝나는지 확인
	if (TagString.EndsWith(TEXT(".Basic")))
	{
		// .Basic을 .Unique로 변경
		TagString.RemoveFromEnd(TEXT(".Basic"));
		TagString.Append(TEXT(".Unique"));

		return UGameplayTagsManager::Get().RequestGameplayTag(FName(*TagString));
	}

	// .Basic이 아닐 경우, 기본 태그를 그대로 반환
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

	EffectAssetTags.Broadcast(TagContainer); // 에셋 태그를 얻으면 작동하는 브로드캐스트.
}
