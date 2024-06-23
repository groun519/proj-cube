// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

UCLASS()
class P_CUBE_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:  
	UPROPERTY(VisibleAnyWhere)
		UStaticMeshComponent* Weapon;

	UPROPERTY(EditDefaultsOnly, Category = "Weapons")
		TArray<UStaticMesh*> WeaponMeshes;

	// 무기 메쉬를 변경할 함수
	UFUNCTION(BlueprintCallable)
		void ChangeWeaponMesh(int32 WeaponIndex);
};
