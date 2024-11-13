// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "P_Cube/CubeAbilityTypes.h"
#include "P_Cube/Actor/CubeHitActor.h"
#include "GameFramework/Actor.h"
#include "CubeHitbox.generated.h"

class UNiagaraSystem;

UCLASS()
class P_CUBE_API ACubeHitbox : public ACubeHitActor
{
	GENERATED_BODY()
	
public:
	ACubeHitbox();

	// 충돌 컴포넌트를 설정하는 함수
	UFUNCTION(BlueprintCallable, Category = "Collision")
	void SetHitboxCollision(UPrimitiveComponent* CollisionComponent);

	UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	FDamageEffectParams DamageEffectParams;

protected:
	virtual void BeginPlay() override;
	void OnHit();
	virtual void Destroyed() override;

	UFUNCTION()
	void OnCollisionOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UPrimitiveComponent> HitboxCollision = nullptr;

private:

};
