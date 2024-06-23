// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"

// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	Weapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WEAPON"));
	RootComponent = Weapon;
	//static ConstructorHelpers::FObjectFinder<UStaticMesh> SK_WEAPON(TEXT("/Game/InfinityBladeWeapons/Weapons/Blade/Swords/Blade_BlackKnight/SK_Blade_BlackKnight.SK_Blade_BlackKnight"));
	//Weapon->SetStaticMesh(SK_WEAPON.Object);
	Weapon->SetCollisionProfileName(TEXT("NoCollision"));



	// 특정 무기 선택 및 사용
	//UStaticMesh* SelectedWeaponMesh = WeaponMeshes[1];
	static ConstructorHelpers::FObjectFinder<UStaticMesh> WeaponMesh1(TEXT("/Game/Blueprints/Items/SM_LongSword"));
	WeaponMeshes.Add(WeaponMesh1.Object); // 0
	Weapon->SetRelativeLocation(FVector(0.0f, 0.0f, 5.0f));
	Weapon->SetRelativeScale3D(FVector(0.1f, 0.1f, 0.1f));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> WeaponMesh2(TEXT("/Game/Blueprints/Items/SM_DoubleAxe"));
	WeaponMeshes.Add(WeaponMesh2.Object);
	//Weapon->SetRelativeTransform(FTransform(FQuat(), FVector(0.0f, 0.0f, 500.0f), FVector(0.1f, 0.1f, 0.1f)));

	Weapon->SetStaticMesh(WeaponMeshes[0]);
}

void AWeapon::ChangeWeaponMesh(int32 WeaponIndex) {
	Weapon->SetStaticMesh(WeaponMeshes[WeaponIndex]);

	if (WeaponIndex == 0) {
		Weapon->SetRelativeLocation(FVector(0.0f, 0.0f, 5.0f));
		Weapon->SetRelativeScale3D(FVector(0.1f, 0.1f, 0.1f));
	}
	else if (WeaponIndex == 1) {
		Weapon->SetRelativeLocation(FVector(0.0f, 0.0f, 75.0f));
		Weapon->SetRelativeScale3D(FVector(0.35f, 0.35f, 0.35f));
	}
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

