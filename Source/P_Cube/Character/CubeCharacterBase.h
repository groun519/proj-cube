// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CubeCharacterBase.generated.h"

UCLASS()
class P_CUBE_API ACubeCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	ACubeCharacterBase();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Combat")
	TObjectPtr<USkeletalMeshComponent> Weapon;
};
