// Fill out your copyright notice in the Description page of Project Settings.


#include "CubeAbilitySystemComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "P_Cube/CubeGameplayTags.h"
#include "P_Cube/AbilitySystem/CubeAbilitySystemLibrary.h"
#include "P_Cube/AbilitySystem/Abilities/CubeGameplayAbility.h"
#include "P_Cube/AbilitySystem/Data/AbilityInfo.h"
#include "P_Cube/AbilitySystem/Data/WeaponInfo.h"
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
			AbilitySpec.DynamicAbilityTags.AddTag(FCubeGameplayTags::Get().Abilities_Status_Equipped);
			GiveAbility(AbilitySpec);
		}
	}
	bStartupAbilitiesGiven = true;
	AbilitiesGivenDelegate.Broadcast();
}

void UCubeAbilitySystemComponent::AddCharacterAbility(const TSubclassOf<UGameplayAbility>& StartupAbility)
{
	FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(StartupAbility, 1);
	if ( const UCubeGameplayAbility* CubeAbility = Cast<UCubeGameplayAbility>(AbilitySpec.Ability) )
	{
		AbilitySpec.DynamicAbilityTags.AddTag(FCubeGameplayTags::Get().InputTag_RMB);
		AbilitySpec.DynamicAbilityTags.AddTag(FCubeGameplayTags::Get().Abilities_Status_Equipped);
		GiveAbility(AbilitySpec);
	}
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

void UCubeAbilitySystemComponent::AbilityInputTagPressed(const FGameplayTag& InputTag)
{
	if ( !InputTag.IsValid() ) return;
	FScopedAbilityListLock ActiveScopeLoc(*this);
	for ( FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities() )
	{
		if ( AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag) )
		{
			AbilitySpecInputPressed(AbilitySpec);
			if ( AbilitySpec.IsActive() )
			{
				InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputPressed, AbilitySpec.Handle, AbilitySpec.ActivationInfo.GetActivationPredictionKey());
			}
		}
	}
}

void UCubeAbilitySystemComponent::AbilityInputTagHeld(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;
	FScopedAbilityListLock ActiveScopeLoc(*this);
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
	FScopedAbilityListLock ActiveScopeLoc(*this);
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag) && AbilitySpec.IsActive() )
		{
			AbilitySpecInputReleased(AbilitySpec);
			InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputReleased, AbilitySpec.Handle, AbilitySpec.ActivationInfo.GetActivationPredictionKey());
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

FGameplayTag UCubeAbilitySystemComponent::GetSlotFromAbilityTag(const FGameplayTag& AbilityTag)
{
	if ( const FGameplayAbilitySpec* Spec = GetSpecFromAbilityTag(AbilityTag) )
	{
		return GetInputTagFromSpec(*Spec);
	}
	return FGameplayTag();
}

bool UCubeAbilitySystemComponent::SlotIsEmpty(const FGameplayTag& Slot)
{
	FScopedAbilityListLock ActiveScopeLoc(*this);
	for ( FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities() )
	{
		if ( AbilityHasSlot(AbilitySpec, Slot) )
		{
			return false;
		}
	}
	return true;
}

bool UCubeAbilitySystemComponent::AbilityHasSlot(const FGameplayAbilitySpec& Spec, const FGameplayTag& Slot)
{
	return Spec.DynamicAbilityTags.HasTagExact(Slot);
}

bool UCubeAbilitySystemComponent::AbilityHasAnySlot(const FGameplayAbilitySpec& Spec)
{
	return Spec.DynamicAbilityTags.HasTag(FGameplayTag::RequestGameplayTag(FName("InputTag")));
}

FGameplayAbilitySpec* UCubeAbilitySystemComponent::GetSpecWithSlot(const FGameplayTag& Slot)
{
	FScopedAbilityListLock ActiveScopeLock(*this);
	for ( FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities() )
	{
		if ( AbilitySpec.DynamicAbilityTags.HasTagExact(Slot) )
		{
			return &AbilitySpec;
		}
	}
	return nullptr;
}

bool UCubeAbilitySystemComponent::IsPassiveAbility(const FGameplayAbilitySpec& Spec) const
{
	const UAbilityInfo* AbilityInfo = UCubeAbilitySystemLibrary::GetAbilityInfo(GetAvatarActor());
	const FGameplayTag AbilityTag = GetAbilityTagFromSpec(Spec);
	const FCubeAbilityInfo& Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);
	const FGameplayTag AbilityType = Info.AbilityType;
	return AbilityType.MatchesTagExact(FCubeGameplayTags::Get().Abilities_Type_Passive);
}

void UCubeAbilitySystemComponent::AssignSlotToAbility(FGameplayAbilitySpec& Spec, const FGameplayTag& Slot)
{
	ClearSlot(&Spec);
	Spec.DynamicAbilityTags.AddTag(Slot);
}

void UCubeAbilitySystemComponent::MulticastActivatePassiveEffect_Implementation(const FGameplayTag& AbilityTag, bool bActivate)
{
	ActivatePassiveEffect.Broadcast(AbilityTag, bActivate);
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

		// 스킬이 처음 추가될 경우: Level로 초기화
		if (GetSpecFromAbilityTag(Info.AbilityTag) == nullptr)
		{
			FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(Info.Ability, Level); 
			AbilitySpec.DynamicAbilityTags.AddTag(FCubeGameplayTags::Get().Abilities_Status_UnLocked);
			GiveAbility(AbilitySpec);
			MarkAbilitySpecDirty(AbilitySpec);

			// 클라이언트에 스킬 상태 전송 (UnLocked 상태로)
			ClientUpdateAbilityStatus(Info.AbilityTag, FCubeGameplayTags::Get().Abilities_Status_UnLocked, Level); // 스킬 레벨도 레벨로 설정.
			continue;
		}

		// 스킬이 이미 존재할 경우: 요구 조건 체크
		bool canEquip = true;
		for ( const FGemToValue& GemMap : Info.EquipRequirement )
		{
			if ( GemMap.Requirement != GemMap.Value )
			{
				canEquip = false;
				break;
			}
		}
		if ( canEquip )
		{
			// 요구치를 모두 충족한 경우: 스킬 활성화
			FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(Info.Ability, Level);
			AbilitySpec.DynamicAbilityTags.AddTag(FCubeGameplayTags::Get().Abilities_Status_Equipped);
			GiveAbility(AbilitySpec);
			MarkAbilitySpecDirty(AbilitySpec);

			// 클라이언트에 스킬 상태 전송 (Equipped 상태로)
			ClientUpdateAbilityStatus(Info.AbilityTag, FCubeGameplayTags::Get().Abilities_Status_Equipped, Level);
		}
		else
		{
			// 요구치를 충족하지 못한 경우: 상태 유지
			ClientUpdateAbilityStatus(Info.AbilityTag, FCubeGameplayTags::Get().Abilities_Status_UnLocked, Level);
		}
	}
}

void UCubeAbilitySystemComponent::ServerAddAbility_Implementation(FGameplayTag AbilityTag)
{
	UAbilityInfo* AbilityInfo = UCubeAbilitySystemLibrary::GetAbilityInfo(GetAvatarActor()); // 캐릭터가 가진 스킬정보를 받아옴

	const FCubeAbilityInfo& Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);
	TSubclassOf<UGameplayAbility> Ability = Info.Ability;

	if ( GetSpecFromAbilityTag(AbilityTag) == nullptr )
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(Ability, 1);
		AbilitySpec.DynamicAbilityTags.AddTag(FCubeGameplayTags::Get().Abilities_Status_UnLocked);
		GiveAbility(AbilitySpec);
		MarkAbilitySpecDirty(AbilitySpec);

		// 클라이언트에 스킬 상태 전송 (UnLocked 상태로)
		ClientUpdateAbilityStatus(AbilityTag, FCubeGameplayTags::Get().Abilities_Status_UnLocked, 1); // 스킬 레벨도 레벨로 설정.
	}
}

void UCubeAbilitySystemComponent::ServerAddAttackAbility_Implementation(FGameplayTag AttackAbilityTag)
{
	UWeaponInfo* WeaponInfo = UCubeAbilitySystemLibrary::GetWeaponInfo(GetAvatarActor()); // 캐릭터가 가진 스킬정보를 받아옴

	const FWeaponInformation& Info = WeaponInfo->FindWeaponInfoForTag(AttackAbilityTag);
	TSubclassOf<UGameplayAbility> AttackAbility = Info.AttackAbility;

	if ( GetSpecFromAbilityTag(AttackAbilityTag) == nullptr )
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AttackAbility, 1);

		AbilitySpec.DynamicAbilityTags.AddTag(FCubeGameplayTags::Get().InputTag_RMB);
		AbilitySpec.DynamicAbilityTags.AddTag(FCubeGameplayTags::Get().Abilities_Status_Equipped);
		GiveAbility(AbilitySpec);
		MarkAbilitySpecDirty(AbilitySpec);

		// 클라이언트에 스킬 상태 전송 (UnLocked 상태로)
		ClientUpdateAbilityStatus(AttackAbilityTag, FCubeGameplayTags::Get().Abilities_Status_Equipped, 1); // 스킬 레벨도 레벨로 설정.
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


		


		bool bStatusValid =  // 스킬을 이미 획득했거나, 획득 중인 상태이면 유효하지 않음.
			Status == GameplayTags.Abilities_Status_Equipped || Status == GameplayTags.Abilities_Status_UnLocked;	

		if ( bStatusValid )
		{

			// Handle activation/deactivation for passive abilities

			if ( !SlotIsEmpty(Slot) ) // 이미 슬롯을 가지고 있다면, 슬롯을 제거
			{
				FGameplayAbilitySpec* SpecWithSlot = GetSpecWithSlot(Slot);
				if ( SpecWithSlot )
				{
					// 능력이 같다면, 리턴
					if ( AbilityTag.MatchesTagExact(GetAbilityTagFromSpec(*SpecWithSlot)) )
					{
						ClientEquipAbility(AbilityTag, GameplayTags.Abilities_Status_Equipped, Slot, PrevSlot);
						return;
					}
					if ( IsPassiveAbility(*SpecWithSlot) )
					{
						MulticastActivatePassiveEffect(GetAbilityTagFromSpec(*SpecWithSlot), false);
						DeactivatePassiveAbility.Broadcast(GetAbilityTagFromSpec(*SpecWithSlot));
					}
					ClearSlot(SpecWithSlot);
				}
			}

			if ( !AbilityHasAnySlot(*AbilitySpec) ) // 아직 슬롯이 없다면(활성화되지 않았다면).
			{
				if ( IsPassiveAbility(*AbilitySpec) )
				{
					TryActivateAbility(AbilitySpec->Handle);
					MulticastActivatePassiveEffect(AbilityTag, true);
				}
			}
			AssignSlotToAbility(*AbilitySpec, Slot);
			MarkAbilitySpecDirty(*AbilitySpec);
		}
		ClientEquipAbility(AbilityTag, GameplayTags.Abilities_Status_Equipped, Slot, PrevSlot);
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
	Spec->DynamicAbilityTags.RemoveTag(Slot);
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
