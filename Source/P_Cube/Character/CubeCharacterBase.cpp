// Fill out your copyright notice in the Description page of Project Settings.


#include "CubeCharacterBase.h"
#include "AbilitySystemComponent.h"
#include "P_Cube/AbilitySystem/CubeAbilitySystemComponent.h"
#include "P_Cube/P_Cube.h"
#include "Components/CapsuleComponent.h"

ACubeCharacterBase::ACubeCharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;

	
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore); // ĸ�� �ݸ��� ī�޶� �浹 �����ϰ� ����. (�÷��̾ ī�޶� ���� �޴� ���� ����)
	GetCapsuleComponent()->SetGenerateOverlapEvents(false); // �浹 �̺�Ʈ �����ϰ� ����.
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore); // �޽� ī�޶� �浹 ���� ����.
	GetMesh()->SetCollisionResponseToChannel(ECC_Projectile, ECR_Overlap); // ����ü ���� ������ Overlap���� ����.
	GetMesh()->SetGenerateOverlapEvents(true); // �浹 �̺�Ʈ �����ϰ� ����.

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

FVector ACubeCharacterBase::GetCombatSocketLocation()
{
	check(Weapon);
	return Weapon->GetSocketLocation(WeaponTipSocketName);
}

void ACubeCharacterBase::InitAbilityActorInfo()
{
}

void ACubeCharacterBase::ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level) const
{
	check(IsValid(GetAbilitySystemComponent()));
	check(GameplayEffectClass);
	FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	ContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(GameplayEffectClass, Level, ContextHandle);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), GetAbilitySystemComponent());
}

void ACubeCharacterBase::InitializeDefaultAttributes() const
{
	ApplyEffectToSelf(DefaultPrimaryAttributes, 1.f);
	ApplyEffectToSelf(DefaultSecondaryAttributes, 1.f);
	ApplyEffectToSelf(DefaultVitalAttributes, 1.f);
}

void ACubeCharacterBase::AddCharacterAbilities()
{
	UCubeAbilitySystemComponent* CubeASC = CastChecked<UCubeAbilitySystemComponent>(AbilitySystemComponent);
	if (!HasAuthority()) return;

	CubeASC->AddCharacterAbilities(StartupAbilities);
}
