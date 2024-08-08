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

	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject); // 클래스 데이터에셋에 접근, 할당.
	FCharacterClassDefaultInfo ClassDefaultInfo = CharacterClassInfo->GetClassDefaultInfo(CharacterClass); // 내부 구조체에 접근

	// 1. Effect Context(효과 컨텍스트)를 생성하여 PrimaryAttributesContextHandle에 저장합니다.
	FGameplayEffectContextHandle PrimaryAttributesContextHandle = ASC->MakeEffectContext();
	// 2. 효과 컨텍스트에 AvatarActor를 소스 객체로 추가합니다.
	//    (효과가 이 객체로부터 발생했음을 나타냅니다)
	PrimaryAttributesContextHandle.AddSourceObject(AvatarActor);
	// 3. 효과 사양(Spec)을 생성하여 PrimaryAttributesSpecHandle에 저장합니다.
	//    - ClassDefaultInfo.PrimaryAttributes: 클래스에 따른 주 능력치 효과
	//    - Level: 레벨
	//    - PrimaryAttributesContextHandle: 효과 컨텍스트
	const FGameplayEffectSpecHandle PrimaryAttributesSpecHandle = ASC->MakeOutgoingSpec(ClassDefaultInfo.PrimaryAttributes, Level, PrimaryAttributesContextHandle);
	// 4. 생성된 효과 사양을 자기 자신(ASC)에게 적용합니다.
	ASC->ApplyGameplayEffectSpecToSelf(*PrimaryAttributesSpecHandle.Data.Get());

	// 같은 방식, SecondaryAttributes (부 능력치)
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
	if (CubeGameMode == nullptr) return nullptr; // 게임모드가 할당되어 있지 않다면, 리턴
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
