// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "P_Cube/Character/CubeCharacterBase.h"

// Sets default values
AWeapon::AWeapon()
{
	PrimaryActorTick.bCanEverTick = true;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->CustomDepthStencilValue = 252.f;
	SetRootComponent(WeaponMesh);
}

void AWeapon::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if ( WeaponTag.IsValid() )
	{
		ChangeWeaponMesh(WeaponTag);
	}
}

void AWeapon::Interact_Implementation(AActor* InteractingActor)
{
	//UE_LOG(LogTemp, Warning, TEXT("Interacting!"));

    ACubeCharacterBase* Character = Cast<ACubeCharacterBase>(InteractingActor);

    if ( Character && Character->InteractMontage )
    {
        Character->PlayInteractMontage(WeaponTag);
    }
}

void AWeapon::ChangeWeaponMesh(FGameplayTag NewWeaponTag)
{

	FWeaponInformation NewWeaponMeshInfo = WeaponInfo->FindWeaponInfoForTag(NewWeaponTag);

	WeaponMesh->SetSkeletalMesh(NewWeaponMeshInfo.WeaponMesh);
	WeaponMesh->SetWorldScale3D(NewWeaponMeshInfo.WeaponOffset.GetScale3D() * 2.f);
}