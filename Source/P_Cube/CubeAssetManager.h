// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "CubeAssetManager.generated.h"

/**
 * 
 */
UCLASS()
class P_CUBE_API UCubeAssetManager : public UAssetManager
{
	GENERATED_BODY()
public:

	static UCubeAssetManager& Get();

protected:

	virtual void StartInitialLoading() override;
};
