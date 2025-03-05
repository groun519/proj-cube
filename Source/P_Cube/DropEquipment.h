// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DropEquipment.generated.h"


UCLASS()
class P_CUBE_API ADropEquipment : public AActor
{
	GENERATED_BODY()
	
public:	
	ADropEquipment();

protected:
	virtual void BeginPlay() override;

public:	
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
