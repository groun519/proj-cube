// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Data/CharacterClassInfo.h"
#include "CubeAbilitySystemLibrary.generated.h"

class UAbilityInfo;
class USkillMenuWidgetController;
class UAbilitySystemComponent;
class UAttributeMenuWidgetController;
class UOverlayWidgetController;
struct FWidgetControllerParams;
/**
 * 
 */
UCLASS()
class P_CUBE_API UCubeAbilitySystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintPure, Category = "CubeAbilitySystemLibrary|WidgetController", meta = (DefaultToSelf = "WorldContextObject"))
	static bool MakeWidgetControllerParams(const UObject* WorldContextObject, FWidgetControllerParams& OutWCParams, ACubeHUD*& OutCubeHUD);

	UFUNCTION(BlueprintPure, Category = "CubeAbilitySystemLibrary|WidgetController", meta = (DefaultToSelf = "WorldContextObject"))
	static UOverlayWidgetController* GetOverlayWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category = "CubeAbilitySystemLibrary|WidgetController", meta = (DefaultToSelf = "WorldContextObject"))
	static UAttributeMenuWidgetController* GetAttributeMenuWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category="CubeAbilitySystemLibrary|WidgetController", meta = (DefaultToSelf = "WorldContextObject"))
	static USkillMenuWidgetController* GetSkillMenuWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "CubeAbilitySystemLibrary|CharacterClassDefaults")
	static void InitializeDefaultAttributes(const UObject* WorldContextObject, ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* ASC);

	UFUNCTION(BlueprintCallable, Category = "CubeAbilitySystemLibrary|CharacterClassDefaults")
	static void GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC, ECharacterClass CharacterClass);

	UFUNCTION(BlueprintCallable, Category = "CubeAbilitySystemLibrary|CharacterClassDefaults")
	static UCharacterClassInfo* GetCharacterClassInfo(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category="AuraAbilitySystemLibrary|CharacterClassDefaults")
	static UAbilityInfo* GetAbilityInfo(const UObject* WorldContextObject);


	UFUNCTION(BlueprintPure, Category = "CubeAbilitySystemLibrary|GameplayEffects")
	static bool IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category = "CubeAbilitySystemLibrary|GameplayEffects")
	static bool IsPhysicalHit(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category = "CubeAbilitySystemLibrary|GameplayEffects")
	static bool IsMagicalHit(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category = "CubeAbilitySystemLibrary|GameplayEffects")
	static bool IsPureHit(const FGameplayEffectContextHandle& EffectContextHandle);


	UFUNCTION(BlueprintCallable, Category = "CubeAbilitySystemLibrary|GameplayEffects")
	static void SetIsCriticalHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bInIsCriticalHit);

	UFUNCTION(BlueprintCallable, Category = "CubeAbilitySystemLibrary|GameplayEffects")
	static void SetIsPhysicalHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bInIsPhysicalHit);

	UFUNCTION(BlueprintCallable, Category = "CubeAbilitySystemLibrary|GameplayEffects")
	static void SetIsMagicalHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bInIsMagicalHit);

	UFUNCTION(BlueprintCallable, Category = "CubeAbilitySystemLibrary|GameplayEffects")
	static void SetIsPureHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bInIsPureHit);

	UFUNCTION(BlueprintCallable, Category = "CubeAbilitySystemLibrary|GameplayMechanics")
	static void GetLivePlayersWithinSphere(const UObject* WorldContextObject, TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorsToIgnore, float Radius, const FVector& SphereOrigin);

	UFUNCTION(BlueprintPure, Category = "CubeAbilitySystemLibrary|GameplayMechanics")
	static bool IsNotFriend(AActor* FirstActor, AActor* SecondActor);

	static int32 GetXPRewardForClassAndLevel(const UObject* WorldContextObject, ECharacterClass CharacterClass, int32 CharacterLevel);
};
