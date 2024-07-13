// Fill out your copyright notice in the Description page of Project Settings.


#include "CubeCharacterBase.h"

ACubeCharacterBase::ACubeCharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	Weapon->SetupAttachment(GetMesh(), FName("WeaponHandSocket"));
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

UAbilitySystemComponent* ACubeCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ACubeCharacterBase::BeginPlay()
{
	Super::BeginPlay();

}