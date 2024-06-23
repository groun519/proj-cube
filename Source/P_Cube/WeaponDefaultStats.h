// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "WeaponDefaultStats.generated.h"


USTRUCT(BlueprintType)
struct FWeaponDefaultStats : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
		float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
		float AttackRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
		float Weight;

	// 추가적인 근접 무기 능력치 정의할 것
};