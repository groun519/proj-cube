// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "RuneInfo.generated.h"

USTRUCT(BlueprintType)
struct FCubeRuneInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag RuneTag = FGameplayTag();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<const UTexture2D> Icon = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FString Description = FString();
};

/**
 * 
 */
UCLASS()
class P_CUBE_API URuneInfo : public UDataAsset
{
	GENERATED_BODY()
public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RuneInformation")
	TArray<FCubeRuneInfo> RuneInformation;

	FCubeRuneInfo FindRuneInfoForTag(const FGameplayTag& RuneTag, bool bLogNotFound = false) const;
};
