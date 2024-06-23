// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
//#include "Engine/DataTable.h"
#include "WeaponDefaultStats.h"
#include "DropEquipment.generated.h"

/*
USTRUCT(BlueprintType)
struct FWeaponStats : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	float AttackRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	float Weight;

	// 추가적인 근접 무기 능력치 정의할 것
};*/


UCLASS()
class P_CUBE_API ADropEquipment : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADropEquipment();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void TriggerBeginOverlap(
			UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult& SweepResult);

	UFUNCTION()
		void TriggerEndOverlap(
			UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex);

	UPROPERTY(EditAnywhere)
	int32 Key;

private:
	UPROPERTY(VisibleAnywhere)
	class UCapsuleComponent* MyCapsuleComponent; // capsule collision component

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* MyMeshComponent; // static mesh component

	UPROPERTY(VisibleAnywhere)
	class USpotLightComponent* MySpotLight; // spot light component

};
