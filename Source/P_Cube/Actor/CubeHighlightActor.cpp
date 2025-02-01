// Fill out your copyright notice in the Description page of Project Settings.


#include "P_Cube/Actor/CubeHighlightActor.h"

// Sets default values
ACubeHighlightActor::ACubeHighlightActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACubeHighlightActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACubeHighlightActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

