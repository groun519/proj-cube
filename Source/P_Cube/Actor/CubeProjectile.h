// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayEffectTypes.h"
#include "CubeProjectile.generated.h"

class UNiagaraSystem;
class USphereComponent;
class UProjectileMovementComponent;

UCLASS()
class P_CUBE_API ACubeProjectile : public AActor
{
	GENERATED_BODY()
	
public:
	ACubeProjectile();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement;

	UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	FGameplayEffectSpecHandle DamageEffectSpecHandle;

protected:
	virtual void BeginPlay() override;
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
	AActor* LastOtherActor = nullptr;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UNiagaraSystem> ImpactEffect; // 제거될 때 생성할 이펙트

	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> ImpactSound; // 제거될 때 생성할 사운드

	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> LoopingSound; // 생성 - 제거까지 반복 재생될 사운드

	UPROPERTY()
	TObjectPtr<UAudioComponent> LoopingSoundComponent;
};
