// Fill out your copyright notice in the Description page of Project Settings.


#include "CubeAssetManager.h"

#include "AbilitySystemGlobals.h"
#include "CubeGameplayTags.h"

UCubeAssetManager& UCubeAssetManager::Get()
{
	check(GEngine);

	UCubeAssetManager* CubeAssetManager = Cast<UCubeAssetManager>(GEngine->AssetManager);
	return *CubeAssetManager;
}

void UCubeAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	FCubeGameplayTags::InitializeNativeGameplayTags();

	// This is required to use Target Data!
	UAbilitySystemGlobals::Get().InitGlobalData(); // gas 관련 시스템 데이터 초기화.
}
