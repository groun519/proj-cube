// Fill out your copyright notice in the Description page of Project Settings.


#include "CubeHitbox.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "P_Cube/AbilitySystem/CubeAbilitySystemLibrary.h"
#include "P_Cube/P_Cube.h"
#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

ACubeHitbox::ACubeHitbox()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
}

void ACubeHitbox::SetHitboxCollision(UPrimitiveComponent* NewCollisionComponent)
{
    // 기존 충돌 컴포넌트가 있으면 이벤트 바인딩 해제
    if ( HitboxCollision )
    {
        HitboxCollision->OnComponentBeginOverlap.RemoveDynamic(this, &ACubeHitbox::OnCollisionOverlap);
        HitboxCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    }

    // 새 충돌 컴포넌트 설정 및 이벤트 바인딩
    HitboxCollision = NewCollisionComponent;

    if ( HitboxCollision )
    {
        HitboxCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
        HitboxCollision->OnComponentBeginOverlap.AddDynamic(this, &ACubeHitbox::OnCollisionOverlap);

		SetRootComponent(HitboxCollision);
		HitboxCollision->SetCollisionObjectType(ECC_Hitbox); // 충돌 타입을 Projectile로 설정
		HitboxCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		HitboxCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
		HitboxCollision->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
		HitboxCollision->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);
		HitboxCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
    }
}

// Called when the game starts or when spawned
void ACubeHitbox::BeginPlay()
{
	Super::BeginPlay();
	SetLifeSpan(LifeTime);
	SetReplicateMovement(true);

	LoopingSoundComponent = UGameplayStatics::SpawnSoundAttached(
		LoopingSound,
		GetRootComponent(),
		NAME_None,
		FVector::ZeroVector,
		EAttachLocation::KeepRelativeOffset,
		true);
}

void ACubeHitbox::OnHit()
{
	UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation(), FRotator::ZeroRotator);
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactEffect, GetActorLocation());
	if ( LoopingSoundComponent ) LoopingSoundComponent->Stop();
	// if (GetLifeSpan() > 0)
	bHit = true;
}

void ACubeHitbox::Destroyed()
{
	if ( !bHit && !HasAuthority() ) OnHit();
	Super::Destroyed();
}

void ACubeHitbox::OnCollisionOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("OnCollisionOverlap triggered with %s"), *OtherActor->GetName());

	if ( DamageEffectParams.SourceAbilitySystemComponent == nullptr ) return;
	for (AActor* ignore : IgnoreActors) 
		if ( OtherActor == ignore ) return;

	if (bIsAttackOnlyTarget)
		if ( OtherActor != TargetActor ) return;
	UE_LOG(LogTemp, Warning, TEXT("OnCollisionOverlap triggered with %s"), *OtherActor->GetName());


	AActor* SourceAvatarActor = DamageEffectParams.SourceAbilitySystemComponent->GetAvatarActor();
	if ( SourceAvatarActor == OtherActor ) return;
	if ( !UCubeAbilitySystemLibrary::IsNotFriend(SourceAvatarActor, OtherActor) && !bDamageTypeIsHeal ) return;
	if ( !bHit ) OnHit();
	UE_LOG(LogTemp, Warning, TEXT("OnCollisionOverlap triggered with %s"), *OtherActor->GetName());


	if (HasAuthority())
	{
		if ( bOnlyPlayer && !OtherActor->ActorHasTag("Player") ) return;

		if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
		{
			/*const bool bKnockback = DamageEffectParams.bKnockback;
			if ( bKnockback )
			{
				FRotator Rotation = GetActorRotation();
				Rotation.Pitch = 45.f;

				const FVector KnockbackDirection = Rotation.Vector();
				const FVector KnockbackForceVec = KnockbackDirection * DamageEffectParams.KnockbackForce;
			}*/

			DamageEffectParams.TargetAbilitySystemComponent = TargetASC;
			UCubeAbilitySystemLibrary::ApplyDamageEffect(DamageEffectParams); // damage 이펙트 적용.
			IgnoreActors.Add(OtherActor);
		}

		if (bDestroyOnOverlap)
		{
			Destroy();
			if (GetLifeSpan() > 0) LoopingSoundComponent->Stop();
		}

		// false <- 제거를 안 함으로서 관통되게 함.
	}
	else bHit = true;
}


