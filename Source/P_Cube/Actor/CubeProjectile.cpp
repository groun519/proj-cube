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
	SetLifeSpan(LifeSpan);
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
	if ( LoopingSoundComponent ) LoopingSoundComponent->Stop();
	// if (GetLifeSpan() > 0)
	bHit = true;
}

void ACubeProjectile::Destroyed()
{
	if ( !bHit && !HasAuthority() ) OnHit();
	Super::Destroyed();
}

void ACubeProjectile::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	for (AActor* ignore : IgnoreActors) 
		if ( OtherActor == ignore ) return;

	if (bIsAttackOnlyTarget)
		if ( OtherActor != TargetActor ) return;


	AActor* SourceAvatarActor = DamageEffectParams.SourceAbilitySystemComponent->GetAvatarActor();
	if ( SourceAvatarActor == OtherActor ) return;
	if ( !UCubeAbilitySystemLibrary::IsNotFriend(SourceAvatarActor, OtherActor) ) return;
	if ( !bHit ) OnHit();


	if (HasAuthority())
	{
		if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
		{
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

AActor* ACubeProjectile::GetInstigatorPlayer() const
{
	return InstigatorPlayer;
}

AActor* ACubeProjectile::GetTargetActor() const
{
	return TargetActor;
}

TArray<AActor*> ACubeProjectile::FindNearestActorsByTag(const FName TagName, const FVector Location, const float Radius, const int32 findingPlayers, const bool bDrawDebugSphere)
{
	TArray<AActor*> NearbyPlayers;
	TArray<FOverlapResult> OverlapResults;

	FCollisionShape CollShape;
	CollShape.SetSphere(Radius);
	FCollisionQueryParams QueryParams;
	//QueryParams.AddIgnoredActor(this);

	bool bOverlap = GetWorld()->OverlapMultiByObjectType(
		OverlapResults,
		Location,
		FQuat::Identity,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_Pawn), // Assuming you are looking for pawns
		CollShape,
		QueryParams
	);

	if ( bOverlap )
	{
		for ( auto& OverlapResult : OverlapResults )
		{
			AActor* OverlappedActor = OverlapResult.GetActor();
			if ( OverlappedActor && OverlappedActor->ActorHasTag(TagName) )
			{
				NearbyPlayers.Add(OverlappedActor);
			}
		}

		// 배열을 위치에 따라 정렬, 가장 가까운 플레이어가 배열의 시작에 오도록
		NearbyPlayers.Sort([ Location ] (AActor& A, AActor& B) -> bool
		{
			return FVector::DistSquared(Location, A.GetActorLocation()) < FVector::DistSquared(Location, B.GetActorLocation());
		});

		// 배열의 크기를 PlayerNum으로 조정
		if ( NearbyPlayers.Num() > findingPlayers )
		{
			NearbyPlayers.SetNum(findingPlayers);
		}
	}

	if ( bDrawDebugSphere )
	{
		DrawDebugSphere(
		GetWorld(),
		Location,
		Radius,
		12, // Segments
		FColor::Yellow,
		false, // Persistent lines
		10.0f, // Duration
		0, // Depth priority
		0.0f // Thickness
		);
	}

	return NearbyPlayers;
}

