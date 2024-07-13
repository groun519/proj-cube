// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "CubeCharacterBase.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;

UCLASS()
class P_CUBE_API ACubeCharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ACubeCharacterBase();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override; // AbilitySytem getter.
	UAttributeSet* GetAttributeSet() const { return AttributeSet; } // AttributeSet getter.

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Combat")
	TObjectPtr<USkeletalMeshComponent> Weapon; // ¹«±â ½ºÄÌ·¹Å» ¸Þ½Ã

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent; // ASC

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet; // AS
};
