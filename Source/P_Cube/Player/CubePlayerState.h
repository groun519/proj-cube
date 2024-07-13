// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "CubePlayerState.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;

/**
 * 
 */
UCLASS() // 멀티플레이어에서의 각 플레이어의 상태 정보 관리
class P_CUBE_API ACubePlayerState : public APlayerState, public IAbilitySystemInterface 
{
	GENERATED_BODY()

public:
	ACubePlayerState();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }

protected:

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;
};
