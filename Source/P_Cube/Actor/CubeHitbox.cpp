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

void ACubeHitbox::SetHitboxCollisionToFan(USphereComponent* SphereComponent, float Angle, float InnerRadius, bool bDebugFan)
{
	// 기존 충돌 컴포넌트가 있으면 이벤트 바인딩 해제
	if ( HitboxCollision )
	{
		HitboxCollision->OnComponentBeginOverlap.RemoveDynamic(this, &ACubeHitbox::OnFanOverlap);
		HitboxCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	// 새 충돌 컴포넌트 설정 및 이벤트 바인딩
	HitboxCollision = SphereComponent;
	SphereComp = SphereComponent;
	bDebugFanRad = bDebugFan;
	Ang = Angle;
	InnerRad = InnerRadius;

	if ( HitboxCollision )
	{
		HitboxCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		HitboxCollision->OnComponentBeginOverlap.AddDynamic(this, &ACubeHitbox::OnFanOverlap);

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

	if ( LinkedAbility )
	{
		ForwardVector = LinkedAbility->GetAvatarActorFromActorInfo()->GetActorForwardVector();
	}

	if ( bDebugFanRad )
	{
		DebugFan();
	}
}

void ACubeHitbox::OnHit()
{
	UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation(), FRotator::ZeroRotator);
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactEffect, GetActorLocation());
	if ( LoopingSoundComponent )
	{
		LoopingSoundComponent->Stop();
		LoopingSoundComponent->DestroyComponent();
	}
	bHit = true;
}

void ACubeHitbox::Destroyed()
{
	if ( LoopingSoundComponent )
	{
		LoopingSoundComponent->Stop();
		LoopingSoundComponent->DestroyComponent();
	}
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
			/** Knockback **/
			const bool bKnockback = DamageEffectParams.Knockback_ForceMagnitude != 0.f;
			if ( bKnockback )
			{
				const FVector ActorLocation = GetActorLocation();
				const FVector AttackerLocation = OtherActor->GetActorLocation();

				const FVector KnockbackDirection = ( AttackerLocation - ActorLocation ).GetSafeNormal();
				const FVector KnockbackForce = KnockbackDirection * DamageEffectParams.Knockback_ForceMagnitude;
				DamageEffectParams.Knockback_Force = KnockbackForce;
			}
			/** end Knockback **/

			/** Grab **/
			const bool bGrab = DamageEffectParams.Grab_ForceMagnitude != 0.f;
			if ( bGrab )
			{
				const FVector ActorLocation = GetActorLocation();
				const FVector AttackerLocation = OtherActor->GetActorLocation();

				const FVector GrabDirection = ( ActorLocation - AttackerLocation ).GetSafeNormal();
				const FVector GrabForce = GrabDirection * DamageEffectParams.Grab_ForceMagnitude;
				DamageEffectParams.Grab_Force = GrabForce;
			}
			/** end Grab **/

			/** Airborne **/
			const bool bAirborn = DamageEffectParams.Airborne_ForceMagnitude != 0.f;
			if ( bAirborn )
			{
				const FVector AirborneDirection = FVector(0, 0, 1);
				const FVector AirborneForce = AirborneDirection * DamageEffectParams.Airborne_ForceMagnitude;
				DamageEffectParams.Airborne_Force = AirborneForce;
			}
			/** end Airborne **/

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

void ACubeHitbox::OnFanOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	FVector ActorLocation = GetActorLocation();
	FVector TargetLocation = OtherActor->GetActorLocation();

	float OtherActorDist = FVector::Dist(ActorLocation, TargetLocation);
	if ( OtherActorDist < InnerRad ) return;

	FVector Direction = ( TargetLocation - ActorLocation ).GetSafeNormal();

	float XAngle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(ForwardVector, Direction)));

	if ( XAngle > Ang / 2 ) return;

	// - - - - - //

	UE_LOG(LogTemp, Warning, TEXT("OnCollisionOverlap triggered with %s"), *OtherActor->GetName());

	if ( DamageEffectParams.SourceAbilitySystemComponent == nullptr ) return;
	for ( AActor* ignore : IgnoreActors )
		if ( OtherActor == ignore ) return;

	if ( bIsAttackOnlyTarget )
		if ( OtherActor != TargetActor ) return;
	UE_LOG(LogTemp, Warning, TEXT("OnCollisionOverlap triggered with %s"), *OtherActor->GetName());


	AActor* SourceAvatarActor = DamageEffectParams.SourceAbilitySystemComponent->GetAvatarActor();
	if ( SourceAvatarActor == OtherActor ) return;
	if ( !UCubeAbilitySystemLibrary::IsNotFriend(SourceAvatarActor, OtherActor) && !bDamageTypeIsHeal ) return;
	if ( !bHit ) OnHit();
	UE_LOG(LogTemp, Warning, TEXT("OnCollisionOverlap triggered with %s"), *OtherActor->GetName());


	if ( HasAuthority() )
	{
		if ( bOnlyPlayer && !OtherActor->ActorHasTag("Player") ) return;

		if ( UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor) )
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

		if ( bDestroyOnOverlap )
		{
			Destroy();
			if ( GetLifeSpan() > 0 ) LoopingSoundComponent->Stop();
		}

		// false <- 제거를 안 함으로서 관통되게 함.
	}
	else bHit = true;
}

void ACubeHitbox::DebugFan()
{
	int32 Segments = 30; // 부채꼴을 구성할 세그먼트 수
	float HalfAngle = Ang / 2.0f; // 부채꼴의 절반 각도
	float SegmentAngle = Ang / Segments; // 각 세그먼트가 차지할 각도

	FVector ActorLocation = GetActorLocation();
	// ForwardVector를 로컬 x축 방향으로 설정

	FVector LastPoint = ActorLocation + ForwardVector.RotateAngleAxis(-HalfAngle, FVector::UpVector) * SphereComp->GetScaledSphereRadius();

	// 부채꼴을 그리기 위해 각 세그먼트를 회전
	for ( int32 i = 1; i <= Segments; ++i )
	{
		float CurrentAngle = -HalfAngle + ( i * SegmentAngle ); // -HalfAngle부터 시작해서 세그먼트만큼 회전
		FVector RotatedVector = ForwardVector.RotateAngleAxis(CurrentAngle, FVector::UpVector); // z축 기준으로 회전
		FVector NextPoint = ActorLocation + RotatedVector * SphereComp->GetScaledSphereRadius();

		// 디버그 라인 그리기
		DrawDebugLine(GetWorld(), LastPoint, NextPoint, FColor::Yellow, false, 2.f, 0, 2.0f);
		LastPoint = NextPoint;
	}

	// 중심에서 부채꼴 양 끝점까지 라인 그리기
	DrawDebugLine(GetWorld(), ActorLocation, ActorLocation + ForwardVector.RotateAngleAxis(-HalfAngle, FVector::UpVector) * SphereComp->GetScaledSphereRadius(), FColor::Yellow, false, 2.f, 0, 2.0f);
	DrawDebugLine(GetWorld(), ActorLocation, ActorLocation + ForwardVector.RotateAngleAxis(HalfAngle, FVector::UpVector) * SphereComp->GetScaledSphereRadius(), FColor::Yellow, false, 2.f, 0, 2.0f);

}
