// Fill out your copyright notice in the Description page of Project Settings.


#include "CubeAbilitySystemLibrary.h"

#include "P_Cube/CubeAbilityTypes.h"
#include "P_Cube/CubeGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "P_Cube/UI/WidgetController/CubeWidgetController.h"
#include "P_Cube/Player/CubePlayerState.h"
#include "P_Cube/UI/HUD/CubeHUD.h"

UOverlayWidgetController* UCubeAbilitySystemLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (ACubeHUD* CubeHUD = Cast<ACubeHUD>(PC->GetHUD()))
		{
			ACubePlayerState* PS = PC->GetPlayerState<ACubePlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();
			const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
			return CubeHUD->GetOverlayWidgetController(WidgetControllerParams);
		}
	}
	return nullptr;
}

UAttributeMenuWidgetController* UCubeAbilitySystemLibrary::GetAttributeMenuWidgetController(const UObject* WorldContextObject)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (ACubeHUD* CubeHUD = Cast<ACubeHUD>(PC->GetHUD()))
		{
			ACubePlayerState* PS = PC->GetPlayerState<ACubePlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();
			const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
			return CubeHUD->GetAttributeMenuWidgetController(WidgetControllerParams);
		}
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

void UCubeAbilitySystemLibrary::GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC)
{
	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	for (TSubclassOf<UGameplayAbility> AbilityClass : CharacterClassInfo->CommonAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		ASC->GiveAbility(AbilitySpec);
	}
}

UCharacterClassInfo* UCubeAbilitySystemLibrary::GetCharacterClassInfo(const UObject* WorldContextObject)
{
	ACubeGameModeBase* CubeGameMode = Cast<ACubeGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (CubeGameMode == nullptr) return nullptr; // ���Ӹ�尡 �Ҵ�Ǿ� ���� �ʴٸ�, ����
	return CubeGameMode->CharacterClassInfo;
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
