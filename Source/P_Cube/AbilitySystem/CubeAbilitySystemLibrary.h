// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Data/CharacterClassInfo.h"
#include "CubeAbilitySystemLibrary.generated.h"

class UAbilitySystemComponent;
class UAttributeMenuWidgetController;
class UOverlayWidgetController;
/**
 * 
 */
UCLASS()
class P_CUBE_API UCubeAbilitySystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintPure, Category = "CubeAbilitySystemLibrary|WidgetController")
	static UOverlayWidgetController* GetOverlayWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category = "CubeAbilitySystemLibrary|WidgetController")
	static UAttributeMenuWidgetController* GetAttributeMenuWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "CubeAbilitySystemLibrary|CharacterClassDefaults")
	static void InitializeDefaultAttributes(const UObject* WorldContextObject, ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* ASC);

	UFUNCTION(BlueprintCallable, Category = "CubeAbilitySystemLibrary|CharacterClassDefaults")
	static void GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC);

	UFUNCTION(BlueprintCallable, Category = "CubeAbilitySystemLibrary|CharacterClassDefaults")
	static UCharacterClassInfo* GetCharacterClassInfo(const UObject* WorldContextObject);


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
};
