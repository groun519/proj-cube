// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "P_Cube/Weapon.h"
#include "CubeCharacterBase.h"
#include "CubeCharacter.generated.h"

UCLASS()
class P_CUBE_API ACubeCharacter : public ACubeCharacterBase
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACubeCharacter();

	UPROPERTY(EditAnywhere)
	bool HaveWeapon;
	int32 WeaponNum;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void SetWeaponVisibility();

	void Attack();
	void MoveDest(const FVector Destination);

	bool IsPlayingMontage(int _n);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animations")
		TArray<UAnimMontage*> BagicAttackMontages;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* WeaponMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCapsuleComponent* WeaponCol;

	AWeapon* CurWeapon;
	int n;
};
