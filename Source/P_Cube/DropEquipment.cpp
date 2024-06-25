// Fill out your copyright notice in the Description page of Project Settings.


#include "DropEquipment.h"
#include "Character/CubeCharacter.h"
#include "Weapon.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SpotLightComponent.h"

#include <Kismet/KismetSystemLibrary.h>


ADropEquipment::ADropEquipment()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// create and set up capsule comp
	MyCapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("MyCapsuleComponent"));
	RootComponent = MyCapsuleComponent;
	MyCapsuleComponent->InitCapsuleSize(50.0f, 100.0f);
	MyCapsuleComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	// create and set up mesh comp
	MyMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MyMeshComponent"));
	MyMeshComponent->SetupAttachment(RootComponent);
	MyMeshComponent->SetCanEverAffectNavigation(false);


	/* set the mesh to a predefined static mesh(replace with your own mesh)
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("StaticMesh'/Game/Path/To/Your/Mesh.YourMesh'"));
	if (MeshAsset.Succeeded())
	{
		MyMeshComponent->SetStaticMesh(MeshAsset.Object);
	}*/


	MySpotLight = CreateDefaultSubobject<USpotLightComponent>(TEXT("MySpotLight"));
	MySpotLight->SetupAttachment(RootComponent);
	MySpotLight->SetIntensity(2000.0f); // 라이트의 강도
	MySpotLight->SetOuterConeAngle(45.0f); // 외부 콘 각도
	MySpotLight->SetAttenuationRadius(1000.0f); // 라이트의 영향 범위

	// Set Spotlight Rotation
	FRotator NewRotation = FRotator(90.0f, 0.0f, 0.0f); // pitch값을 90으로 설정
	MySpotLight->SetRelativeRotation(NewRotation);




	Key = 0;
	//int Damage = 1;
}

// Called when the game starts or when spawned
void ADropEquipment::BeginPlay()
{
	Super::BeginPlay();
	

	MyCapsuleComponent->OnComponentBeginOverlap.AddDynamic(this, &ADropEquipment::TriggerBeginOverlap);
	MyCapsuleComponent->OnComponentEndOverlap.AddDynamic(this, &ADropEquipment::TriggerEndOverlap);
}

// Called every frame
void ADropEquipment::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void ADropEquipment::TriggerBeginOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	//UE_LOG(LogTemp, Warning, TEXT("콜리전 발생!"));

	//UKismetSystemLibrary::PrintString(GetWorld(), TEXT("Choongdol SeongGong!"), true, true, FLinearColor::Green, 2.0f);


	ACubeCharacter* OtherActorAsSpecificType = Cast<ACubeCharacter>(OtherActor);

	if (OtherActorAsSpecificType)
	{
		// Check if the HaveWeapon variable is false
		if (!OtherActorAsSpecificType->HaveWeapon)
		{
			// Set the Weapon variable to 1
			OtherActorAsSpecificType->WeaponNum = Key;
			OtherActorAsSpecificType->HaveWeapon = true;
			OtherActorAsSpecificType->SetWeaponVisibility();

			// Now you can do additional logic if needed

			//UKismetSystemLibrary::PrintString(GetWorld(), TEXT("Moo-Gi Oiisii!"), true, true, FLinearColor::Green, 2.0f);
			Destroy();
		}
	}
}












void ADropEquipment::TriggerEndOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	
}