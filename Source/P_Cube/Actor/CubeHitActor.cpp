// Fill out your copyright notice in the Description page of Project Settings.


#include "CubeHitActor.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/AudioComponent.h"
#include "P_Cube/AbilitySystem/CubeAbilitySystemLibrary.h"
#include "Kismet/GameplayStatics.h"

AActor* ACubeHitActor::GetInstigatorPlayer() const
{
	return InstigatorPlayer;
}

AActor* ACubeHitActor::GetTargetActor() const
{
	return TargetActor;
}

/*TArray<AActor*> ACubeHitActor::FindNearestPlayers(const FVector Location, const float Radius, const int32 findingPlayers, const bool bDrawDebugSphere)
{
	TArray<AActor*> NearbyPlayers;
	TArray<FOverlapResult> OverlapResults;

	FCollisionShape CollShape;
	CollShape.SetSphere(Radius);
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

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
			if ( OverlappedActor && OverlappedActor->ActorHasTag(FName("Player")) )
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
}*/

TArray<AActor*> ACubeHitActor::FindNearestActorsByTag(const FName TagName, const FVector Location, const float Radius, const int32 findingPlayers, const bool bDrawDebugSphere)
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


void ACubeHitActor::ResetIgnoreActors()
{
	IgnoreActors.Empty();
}