// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "P_Cube/CubeAbilityTypes.h"
#include "GameFramework/Actor.h"
#include "CubeHitbox.generated.h"

class UNiagaraSystem;

UCLASS()
class P_CUBE_API ACubeHitbox : public AActor
{
	GENERATED_BODY()
	
public:
	ACubeHitbox();

	// 충돌 컴포넌트를 설정하는 함수
	UFUNCTION(BlueprintCallable, Category = "Collision")
	void SetHitboxCollision(UPrimitiveComponent* CollisionComponent);

	UFUNCTION(BlueprintCallable, Category = "Collision")
	void ResetIgnoreActors();

	UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	FDamageEffectParams DamageEffectParams;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Ability")
	UGameplayAbility* LinkedAbility;

	bool bDamageTypeIsHeal = false;

	AActor* InstigatorPlayer;
	bool bIsAttackOnlyTarget = false;

	UPROPERTY(BlueprintReadWrite)
	AActor* TargetActor;

	void SetOnlyPlayer(const bool OnlyPlayer);

protected:
	virtual void BeginPlay() override;
	void OnHit();
	virtual void Destroyed() override;
	
	UPrimitiveComponent* HitboxCollision;

	UFUNCTION()
	void OnCollisionOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:

	UPROPERTY(EditDefaultsOnly)
	float LifeSpan = 15.f;

	UPROPERTY(EditDefaultsOnly)
	bool bDestroyOnOverlap = false;

	bool bHit = false;
	AActor* LastOtherActor = nullptr;

	TArray<AActor*> IgnoreActors;

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

	bool bOnlyPlayer = false;
};
