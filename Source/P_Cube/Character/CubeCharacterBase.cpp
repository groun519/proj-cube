// Fill out your copyright notice in the Description page of Project Settings.


#include "CubeCharacterBase.h"
#include "AbilitySystemComponent.h"
#include "P_Cube/AbilitySystem/CubeAbilitySystemComponent.h"
#include "P_Cube/P_Cube.h"
#include "Components/CapsuleComponent.h"

ACubeCharacterBase::ACubeCharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;

	
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore); // 캡슐 콜리전 카메라 충돌 무시하게 설정. (플레이어에 카메라가 영향 받는 것을 방지)
	GetCapsuleComponent()->SetGenerateOverlapEvents(false); // 충돌 이벤트 가능하게 설정.
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore); // 메쉬 카메라 충돌 무시 설정.
	GetMesh()->SetCollisionResponseToChannel(ECC_Projectile, ECR_Overlap); // 투사체 접촉 판정을 Overlap으로 설정.
	GetMesh()->SetGenerateOverlapEvents(true); // 충돌 이벤트 가능하게 설정.

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
