// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "P_Cube/UI/Widget/CubeUserWidget.h"
#include "GameplayTagContainer.h"
#include "P_Cube/Character/CubeCharacterBase.h"
#include "WeaponWidget.generated.h"


/**
 * 
 */
UCLASS()
class P_CUBE_API UWeaponWidget : public UCubeUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void SetPlayerWeaponTag(ACubeCharacterBase* Player, FGameplayTag WeaponTag);
};
