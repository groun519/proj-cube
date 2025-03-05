// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayTagContainer.h"
#include "P_Cube/Interaction/InteractableInterface.h"
#include "P_Cube/Interaction/HighlightInterface.h"
#include "P_Cube/Character/CubeCharacterBase.h"
#include "P_Cube/AbilitySystem/Data/WeaponInfo.h"
#include "Weapon.generated.h"

class UWidgetComponent;

UCLASS()
class P_CUBE_API AWeapon : public AActor, public IInteractableInterface, public IHighlightInterface
{
	GENERATED_BODY()
	
public:	
	AWeapon();

protected:
	UFUNCTION()
	virtual void OnConstruction(const FTransform& Transform) override;

public:  
	UPROPERTY(EditDefaultsOnly, Category = "Weapon DataAsset")
	TObjectPtr<UWeaponInfo> WeaponInfo;

	UPROPERTY(EditAnywhere, Category = "Weapon Settings")
	FGameplayTag WeaponTag = FGameplayTag();

	UPROPERTY(EditDefaultsOnly, Category = "Weapon Settings")
	float InteractionDistance = 5.f;

	/** Interactable Interface **/
	virtual void Interact_Implementation(AActor* InteractingActor) override;
	/** end Interactable Interface **/

	// 태그 기반으로 WeaponInfo에서 받아올 스켈레탈 메쉬
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<USkeletalMeshComponent> WeaponMesh;

private:
	

	void ChangeWeaponMesh(FGameplayTag NewWeaponTag);
};
