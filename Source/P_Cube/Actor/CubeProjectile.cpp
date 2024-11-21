// Fill out your copyright notice in the Description page of Project Settings.


#include "CubeProjectile.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "P_Cube/AbilitySystem/CubeAbilitySystemLibrary.h"
#include "P_Cube/P_Cube.h"
#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

ACubeProjectile::ACubeProjectile()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	SetRootComponent(Sphere);
	Sphere->SetCollisionObjectType(ECC_Projectile); // 충돌 타입을 Projectile로 설정
	Sphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Sphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	Sphere->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	Sphere->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);
	Sphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");
	ProjectileMovement->InitialSpeed = 550.f;
	ProjectileMovement->MaxSpeed = 550.f;
	ProjectileMovement->ProjectileGravityScale = 0.f;
}

// Called when the game starts or when spawned
void ACubeProjectile::BeginPlay()
{
	Super::BeginPlay();
	SetLifeSpan(LifeTime);
	SetReplicateMovement(true);
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &ACubeProjectile::OnSphereOverlap);

	LoopingSoundComponent = UGameplayStatics::SpawnSoundAttached(
		LoopingSound, 
		GetRootComponent(), 
		NAME_None, 
		FVector::ZeroVector, 
		EAttachLocation::KeepRelativeOffset, 
		true);
}

void ACubeProjectile::OnHit()
{
	UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation(), FRotator::ZeroRotator);
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactEffect, GetActorLocation());
	if ( LoopingSoundComponent )
	{
		LoopingSoundComponent->Stop();
		LoopingSoundComponent->DestroyComponent();
	}
	// if (GetLifeSpan() > 0)
	bHit = true;
}

void ACubeProjectile::Destroyed()
{
	if ( LoopingSoundComponent )
	{
		LoopingSoundComponent->Stop();
		LoopingSoundComponent->DestroyComponent();
	}
	if ( !bHit && !HasAuthority() ) OnHit();
	Super::Destroyed();
}

void ACubeProjectile::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if ( DamageEffectParams.SourceAbilitySystemComponent == nullptr ) return;
	for (AActor* ignore : IgnoreActors) 
		if ( OtherActor == ignore ) return;

	if (bIsAttackOnlyTarget)
		if ( OtherActor != TargetActor ) return;


	AActor* SourceAvatarActor = DamageEffectParams.SourceAbilitySystemComponent->GetAvatarActor();
	if ( SourceAvatarActor == OtherActor ) return;
	if ( !UCubeAbilitySystemLibrary::IsNotFriend(SourceAvatarActor, OtherActor) && !bDamageTypeIsHeal ) return;
	if ( !bHit ) OnHit();


	if (HasAuthority())
	{
		if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
		{
			/** Knockback **/
			const bool bKnockback = DamageEffectParams.Knockback_ForceMagnitude != 0.f;
			if ( bKnockback )
			{
				FRotator Rotation = GetActorRotation();
				Rotation.Pitch = 0.f;

				const FVector KnockbackDirection = Rotation.Vector();
				const FVector KnockbackForce = KnockbackDirection * DamageEffectParams.Knockback_ForceMagnitude;
				DamageEffectParams.Knockback_Force = KnockbackForce;
			}
			/** end Knockback **/

			/** Grab **/
			const bool bGrab = DamageEffectParams.Grab_ForceMagnitude != 0.f;
			if ( bGrab )
			{
				FRotator Rotation = GetActorRotation();
				Rotation.Pitch = 0.f;

				const FVector GrabDirection = Rotation.Vector();
				const FVector GrabForce = GrabDirection * DamageEffectParams.Grab_ForceMagnitude;
				DamageEffectParams.Grab_Force = GrabForce;
			}
			/** end Grab **/

			/** Airborne **/
			const bool bAirborne = DamageEffectParams.Airborne_ForceMagnitude != 0.f;
			if ( bAirborne )
			{
				FRotator Rotation = GetActorRotation();
				Rotation.Pitch = 90.f;

				const FVector AirborneDirection = Rotation.Vector();
				const FVector AirborneForce = AirborneDirection * DamageEffectParams.Airborne_ForceMagnitude;
				DamageEffectParams.Airborne_Force = AirborneForce;
			}
			/** end Airborne **/

			DamageEffectParams.TargetAbilitySystemComponent = TargetASC;
			UCubeAbilitySystemLibrary::ApplyDamageEffect(DamageEffectParams); // damage 이펙트 적용.
		}

		if (bDestroyOnOverlap) 
		{
			Destroy();
			if (GetLifeSpan() > 0) LoopingSoundComponent->Stop();
		}
		else // false <- 제거를 안 함으로서 관통되게 함.
		{
			IgnoreActors.Add(OtherActor);
		}
	}
	else bHit = true;
}

