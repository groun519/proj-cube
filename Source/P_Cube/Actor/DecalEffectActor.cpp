// Fill out your copyright notice in the Description page of Project Settings.


#include "P_Cube/Actor/DecalEffectActor.h"
#include "Components/DecalComponent.h"

// Sets default values
ADecalEffectActor::ADecalEffectActor()
{
	PrimaryActorTick.bCanEverTick = true;

	DecalComp = CreateDefaultSubobject<UDecalComponent>("MagicCircleDecal");
	DecalComp->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void ADecalEffectActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADecalEffectActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

