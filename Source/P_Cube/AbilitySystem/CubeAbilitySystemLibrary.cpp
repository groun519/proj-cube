// Fill out your copyright notice in the Description page of Project Settings.


#include "CubeAbilitySystemLibrary.h"

#include "P_Cube/CubeAbilityTypes.h"
#include "P_Cube/CubeGameModeBase.h"
#include "P_Cube/Interaction/CombatInterface.h"
#include "Kismet/GameplayStatics.h"
#include "P_Cube/Player/CubePlayerState.h"
#include "P_Cube/UI/HUD/CubeHUD.h"
#include "P_Cube/UI/WidgetController/CubeWidgetController.h"

bool UCubeAbilitySystemLibrary::MakeWidgetControllerParams(const UObject* WorldContextObject, FWidgetControllerParams& OutWCParams, ACubeHUD*& OutCubeHUD)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		OutCubeHUD = Cast<ACubeHUD>(PC->GetHUD());
		if (OutCubeHUD)
		{
			ACubePlayerState* PS = PC->GetPlayerState<ACubePlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();

			OutWCParams.AttributeSet = AS;
			OutWCParams.AbilitySystemComponent = ASC;
			OutWCParams.PlayerState = PS;
			OutWCParams.PlayerController = PC;
			return true;
		}
	}
	return false;
}

UOverlayWidgetController* UCubeAbilitySystemLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{
	FWidgetControllerParams WCParams;
	ACubeHUD* CubeHUD = nullptr;
	if (MakeWidgetControllerParams(WorldContextObject, WCParams, CubeHUD))
	{
		return CubeHUD->GetOverlayWidgetController(WCParams);
	}
	return nullptr;
}

UAttributeMenuWidgetController* UCubeAbilitySystemLibrary::GetAttributeMenuWidgetController(const UObject* WorldContextObject)
{
	FWidgetControllerParams WCParams;
	ACubeHUD* CubeHUD = nullptr;
	if (MakeWidgetControllerParams(WorldContextObject, WCParams, CubeHUD))
	{
		return CubeHUD->GetAttributeMenuWidgetController(WCParams);
	}
	return nullptr;
}

USkillMenuWidgetController* UCubeAbilitySystemLibrary::GetSkillMenuWidgetController(const UObject* WorldContextObject)
{
	FWidgetControllerParams WCParams;
	ACubeHUD* CubeHUD = nullptr;
	if (MakeWidgetControllerParams(WorldContextObject, WCParams, CubeHUD))
	{
		return CubeHUD->GetSkillMenuWidgetController(WCParams);
	}
	return nullptr;
}

void UCubeAbilitySystemLibrary::InitializeDefaultAttributes(const UObject* WorldContextObject, ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* ASC)
{
	AActor* AvatarActor = ASC->GetAvatarActor();

	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject); // Ŭ���� �����Ϳ��¿� ����, �Ҵ�.
	FCharacterClassDefaultInfo ClassDefaultInfo = CharacterClassInfo->GetClassDefaultInfo(CharacterClass); // ���� ����ü�� ����

	// 1. Effect Context(ȿ�� ���ؽ�Ʈ)�� �����Ͽ� PrimaryAttributesContextHandle�� �����մϴ�.
	FGameplayEffectContextHandle PrimaryAttributesContextHandle = ASC->MakeEffectContext();
	// 2. ȿ�� ���ؽ�Ʈ�� AvatarActor�� �ҽ� ��ü�� �߰��մϴ�.
	//    (ȿ���� �� ��ü�κ��� �߻������� ��Ÿ���ϴ�)
	PrimaryAttributesContextHandle.AddSourceObject(AvatarActor);
	// 3. ȿ�� ���(Spec)�� �����Ͽ� PrimaryAttributesSpecHandle�� �����մϴ�.
	//    - ClassDefaultInfo.PrimaryAttributes: Ŭ������ ���� �� �ɷ�ġ ȿ��
	//    - Level: ����
	//    - PrimaryAttributesContextHandle: ȿ�� ���ؽ�Ʈ
	const FGameplayEffectSpecHandle PrimaryAttributesSpecHandle = ASC->MakeOutgoingSpec(ClassDefaultInfo.PrimaryAttributes, Level, PrimaryAttributesContextHandle);
	// 4. ������ ȿ�� ����� �ڱ� �ڽ�(ASC)���� �����մϴ�.
	ASC->ApplyGameplayEffectSpecToSelf(*PrimaryAttributesSpecHandle.Data.Get());

	// ���� ���, SecondaryAttributes (�� �ɷ�ġ)
	FGameplayEffectContextHandle SecondaryAttributesContextHandle = ASC->MakeEffectContext();
	SecondaryAttributesContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle SecondaryAttributesSpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->SecondaryAttributes, Level, SecondaryAttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*SecondaryAttributesSpecHandle.Data.Get());

	// VitalAttributes
	FGameplayEffectContextHandle VitalAttributesContextHandle = ASC->MakeEffectContext();
	VitalAttributesContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle VitalAttributesSpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->VitalAttributes, Level, VitalAttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*VitalAttributesSpecHandle.Data.Get());
}

void UCubeAbilitySystemLibrary::GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC, ECharacterClass CharacterClass)
{
	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	if (CharacterClassInfo == nullptr) return;
	for (TSubclassOf<UGameplayAbility> AbilityClass : CharacterClassInfo->CommonAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		ASC->GiveAbility(AbilitySpec);
	}
	const FCharacterClassDefaultInfo& DefaultInfo = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);
	for (TSubclassOf<UGameplayAbility> AbilityClass : DefaultInfo.StartupAbilities)
	{
		if (ASC->GetAvatarActor()->Implements<UCombatInterface>())
		{
			FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, ICombatInterface::Execute_GetPlayerLevel(ASC->GetAvatarActor()));
			ASC->GiveAbility(AbilitySpec);
		}
	}
}

UCharacterClassInfo* UCubeAbilitySystemLibrary::GetCharacterClassInfo(const UObject* WorldContextObject)
{
	const ACubeGameModeBase* CubeGameMode = Cast<ACubeGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (CubeGameMode == nullptr) return nullptr; // ���Ӹ�尡 �Ҵ�Ǿ� ���� �ʴٸ�, ����
	return CubeGameMode->CharacterClassInfo;
}

UAbilityInfo* UCubeAbilitySystemLibrary::GetAbilityInfo(const UObject* WorldContextObject)
{
	const ACubeGameModeBase* CubeGameMode = Cast<ACubeGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (CubeGameMode == nullptr) return nullptr;
	return CubeGameMode->AbilityInfo;
}

bool UCubeAbilitySystemLibrary::IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FCubeGameplayEffectContext* CubeEffectContext = static_cast<const FCubeGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return CubeEffectContext->IsCriticalHit();
	}
	return false;
}

bool UCubeAbilitySystemLibrary::IsPhysicalHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FCubeGameplayEffectContext* CubeEffectContext = static_cast<const FCubeGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return CubeEffectContext->IsPhysicalHit();
	}
	return false;
}

bool UCubeAbilitySystemLibrary::IsMagicalHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FCubeGameplayEffectContext* CubeEffectContext = static_cast<const FCubeGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return CubeEffectContext->IsMagicalHit();
	}
	return false;
}

bool UCubeAbilitySystemLibrary::IsPureHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FCubeGameplayEffectContext* CubeEffectContext = static_cast<const FCubeGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return CubeEffectContext->IsPureHit();
	}
	return false;
}

void UCubeAbilitySystemLibrary::SetIsCriticalHit(UPARAM(ref)FGameplayEffectContextHandle& EffectContextHandle, bool bInIsCriticalHit)
{
	if (FCubeGameplayEffectContext* CubeEffectContext = static_cast<FCubeGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		CubeEffectContext->SetIsCriticalHit(bInIsCriticalHit);
	}
}

void UCubeAbilitySystemLibrary::SetIsPhysicalHit(UPARAM(ref)FGameplayEffectContextHandle& EffectContextHandle, bool bInIsPhysicalHit)
{
	if (FCubeGameplayEffectContext* CubeEffectContext = static_cast<FCubeGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		CubeEffectContext->SetIsPhysicalHit(bInIsPhysicalHit);
	}
}

void UCubeAbilitySystemLibrary::SetIsMagicalHit(UPARAM(ref)FGameplayEffectContextHandle& EffectContextHandle, bool bInIsMagicalHit)
{
	if (FCubeGameplayEffectContext* CubeEffectContext = static_cast<FCubeGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		CubeEffectContext->SetIsMagicalHit(bInIsMagicalHit);
	}
}

void UCubeAbilitySystemLibrary::SetIsPureHit(UPARAM(ref)FGameplayEffectContextHandle& EffectContextHandle, bool bInIsPureHit)
{
	if (FCubeGameplayEffectContext* CubeEffectContext = static_cast<FCubeGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		CubeEffectContext->SetIsPureHit(bInIsPureHit);
	}
}

void UCubeAbilitySystemLibrary::GetLivePlayersWithinSphere(const UObject* WorldContextObject, TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorsToIgnore, float Radius, const FVector& SphereOrigin)
{ // ���� �޽� ��� �÷��̾� Ž��
	FCollisionQueryParams SphereParams;
	SphereParams.AddIgnoredActors(ActorsToIgnore);

	if (const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		TArray<FOverlapResult> Overlaps;
		World->OverlapMultiByObjectType(Overlaps, SphereOrigin, FQuat::Identity, FCollisionObjectQueryParams(FCollisionObjectQueryParams::InitType::AllDynamicObjects), FCollisionShape::MakeSphere(Radius), SphereParams);
		for (FOverlapResult& Overlap : Overlaps)
		{
			if (Overlap.GetActor()->Implements<UCombatInterface>() && !ICombatInterface::Execute_IsDead(Overlap.GetActor()))
			{
				OutOverlappingActors.AddUnique(ICombatInterface::Execute_GetAvatar(Overlap.GetActor()));
			}
		}
	}
}

bool UCubeAbilitySystemLibrary::IsNotFriend(AActor* FirstActor, AActor* SecondActor)
{
	const bool bBothArePlayers = false; //FirstActor->ActorHasTag(FName("Player")) && SecondActor->ActorHasTag(FName("Player"));
	const bool bBothAreEnemies = FirstActor->ActorHasTag(FName("Enemy")) && SecondActor->ActorHasTag(FName("Enemy"));
	const bool bFriends = bBothArePlayers || bBothAreEnemies;
	return !bFriends;
}

int32 UCubeAbilitySystemLibrary::GetXPRewardForClassAndLevel(const UObject* WorldContextObject, ECharacterClass CharacterClass, int32 CharacterLevel)
{
	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	if (CharacterClassInfo == nullptr) return 0;

	const FCharacterClassDefaultInfo& Info = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);
	const float XPReward = Info.XPReward.GetValueAtLevel(CharacterLevel);

	return static_cast<int32>(XPReward);
}
