// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "P_Cube/CubeAbilityTypes.h"
#include "GameFramework/Actor.h"
#include "CubeHitActor.generated.h"

class UNiagaraSystem;

UCLASS()
class P_CUBE_API ACubeHitActor : public AActor
{
	GENERATED_BODY()
	
public:
	

protected:
	TArray<AActor*> IgnoreActors;

private:
	UFUNCTION(BlueprintCallable)
	TArray<AActor*> FindNearestPlayers(const FVector Location ,const float Radius, const int32 findingPlayers, const bool bDrawDebugSphere);
};
