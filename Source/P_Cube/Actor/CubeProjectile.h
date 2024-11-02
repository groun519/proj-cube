// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "P_Cube/CubeAbilityTypes.h"
#include "P_Cube/Actor/CubeHitActor.h"
#include "GameFramework/Actor.h"
#include "CubeProjectile.generated.h"

class UNiagaraSystem;
class USphereComponent;
class UProjectileMovementComponent;

UCLASS()
class P_CUBE_API ACubeProjectile : public ACubeHitActor
{
	GENERATED_BODY()
	
public:
	ACubeProjectile();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement;

	UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	FDamageEffectParams DamageEffectParams;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Ability")
	UGameplayAbility* LinkedAbility;

	bool bDamageTypeIsHeal = false;

	AActor* InstigatorPlayer;
	bool bIsAttackOnlyTarget = false;

	UPROPERTY(BlueprintReadWrite)
	AActor* TargetActor;

protected:
	virtual void BeginPlay() override;
	void OnHit();
	virtual void Destroyed() override;

	UFUNCTION()
	void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USphereComponent> Sphere;

private:

	UPROPERTY(EditDefaultsOnly)
	float LifeSpan = 15.f;

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

	UFUNCTION(BlueprintCallable)
	TArray<AActor*> FindNearestActorsByTag(const FName TagName, const FVector Location, const float Radius, const int32 findingPlayers, const bool bDrawDebugSphere);
};
