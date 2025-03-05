// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "WeaponInfo.generated.h"

class UGameplayAbility;

USTRUCT(BlueprintType)
struct FAdditionalAttribute
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag AttributeTag = FGameplayTag();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Value = 0.f;
};

USTRUCT(BlueprintType)
struct FWeaponInformation
{
	GENERATED_BODY()

	// 데이터를 쉽게 가져오게 하는 역할을 해 줄 무기 태그
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag WeaponTag = FGameplayTag();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<const UTexture2D> Icon = nullptr;

	// 무기 메쉬
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<USkeletalMesh> WeaponMesh;

	// 무기 오프셋 (트랜스폼 세부조정)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FTransform WeaponOffset;

	// 플레이어가 가지게 될 기본 범위 능력치
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Range = 0.f;

	// 이동속도 계수 (플레이어 기본 이동속도에 곱해짐. ex : 0.75)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float MovementSpeed_Coef = 0.f;

	// 공격속도 계수 (플레이어 기본 공격속도에 곱해짐. ex : 1.25)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float AttackSpeed_Coef = 0.f;

	// 무기가 추가적으로 제공할 어트리부트
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FAdditionalAttribute> AdditionalAttributes;

	// 무기가 기본적으로 제공할 버프나 디버프 이펙트
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag PassiveEffectTag = FGameplayTag();

	// 무기가 기본적으로 제공할 패시브 어빌리티
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag PassiveAbilityTag = FGameplayTag();

	// 무기가 기본적으로 제공할 Attack(평타) 어빌리티
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayAbility> AttackAbility;

	// 무기가 기본적으로 제공할 Attack(평타) 어빌리티
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag AttackAbilityTag = FGameplayTag();
};

/**
 * 
 */
UCLASS()
class P_CUBE_API UWeaponInfo : public UDataAsset
{
	GENERATED_BODY()
public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WeaponInformation")
	TArray<FWeaponInformation> WeaponInformation;

	FWeaponInformation FindWeaponInfoForTag(const FGameplayTag& WeaponTag, bool bLogNotFound = false) const;
};
