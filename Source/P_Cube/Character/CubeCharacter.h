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

	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	UPROPERTY(EditAnywhere)
	bool HaveWeapon;
	int32 WeaponNum;

	void SetWeaponVisibility();

	void Attack();
	void MoveDest(const FVector Destination);

	bool IsPlayingMontage(int _n);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animations")
	TArray<UAnimMontage*> BagicAttackMontages;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* WeaponMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCapsuleComponent* WeaponCol;

	void InitAbilityActorInfo();

	AWeapon* CurWeapon;
	int n;
};
