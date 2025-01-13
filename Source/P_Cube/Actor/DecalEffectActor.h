// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DecalEffectActor.generated.h"

UCLASS()
class P_CUBE_API ADecalEffectActor : public AActor
{
	GENERATED_BODY()

public:	
	ADecalEffectActor();
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UDecalComponent> DecalComp;

protected:
	virtual void BeginPlay() override;
};
