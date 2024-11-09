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
	
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Ability")
	UGameplayAbility* LinkedAbility;

	AActor* InstigatorPlayer;

	UPROPERTY(BlueprintReadWrite)
	AActor* TargetActor;

	bool bDamageTypeIsHeal = false;

	bool bIsAttackOnlyTarget = false;

	bool bOnlyPlayer = false;

protected:
	TArray<AActor*> IgnoreActors;

	UPROPERTY(EditDefaultsOnly)
	float LifeTime = 15.f;

	UPROPERTY(EditDefaultsOnly)
	bool bDestroyOnOverlap = false;

	bool bHit = false;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UNiagaraSystem> ImpactEffect; // 제거될 때 생성할 이펙트

	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> ImpactSound; // 제거될 때 생성할 사운드

	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> LoopingSound; // 생성 - 제거까지 반복 재생될 사운드

	UPROPERTY()
	TObjectPtr<UAudioComponent> LoopingSoundComponent;

	UFUNCTION(BlueprintCallable)
	AActor* GetInstigatorPlayer() const;

	UFUNCTION(BlueprintCallable)
	AActor* GetTargetActor() const;

private:

	/*UFUNCTION(BlueprintCallable)    <- 아래 함수 이전 버전인데 일단 남겨둠
	TArray<AActor*> FindNearestPlayers(
		const FVector Location,
		const float Radius, 
		const int32 findingPlayers, 
		const bool bDrawDebugSphere
	);*/

	UFUNCTION(BlueprintCallable, Category = "Find")
	TArray<AActor*> FindNearestActorsByTag(
		const FName TagName,
		const FVector Location,
		const float Radius,
		const int32 findingPlayers,
		const bool bDrawDebugSphere
	);

	UFUNCTION(BlueprintCallable, Category = "Collision")
	void ResetIgnoreActors();

};
