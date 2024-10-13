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

	AActor* InstigatorPlayer;
	bool bIsAttackOnlyTarget = false;

	UPROPERTY(BlueprintReadWrite)
	AActor* TargetActor;

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

	TArray<AActor*> IgnoreActors;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UNiagaraSystem> ImpactEffect; // ���ŵ� �� ������ ����Ʈ

	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> ImpactSound; // ���ŵ� �� ������ ����

	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> LoopingSound; // ���� - ���ű��� �ݺ� ����� ����

	UPROPERTY()
	TObjectPtr<UAudioComponent> LoopingSoundComponent;

	UFUNCTION(BlueprintCallable)
	AActor* GetInstigatorPlayer() const;

	UFUNCTION(BlueprintCallable)
	AActor* GetTargetActor() const;
};
