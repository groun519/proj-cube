// Fill out your copyright notice in the Description page of Project Settings.


#include "CubeHitActor.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/AudioComponent.h"
#include "P_Cube/AbilitySystem/CubeAbilitySystemLibrary.h"
#include "NiagaraComponent.h"
#include "Kismet/GameplayStatics.h"

ACubeHitActor::ACubeHitActor()
{
	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> WarningEffectObj_Circle(TEXT(
		"NiagaraSystem'/Game/Assets/WarningAsset/Effect/NS_WarningCircle.NS_WarningCircle'"
	));
	WarningEffect_Circle = WarningEffectObj_Circle.Object;

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> WarningEffectObj_Square(TEXT(
		"NiagaraSystem'/Game/Assets/WarningAsset/Effect/NS_WarningSquare.NS_WarningSquare'"
	));
	WarningEffect_Square = WarningEffectObj_Square.Object;

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> WarningEffectObj_Sector(TEXT(
		"NiagaraSystem'/Game/Assets/WarningAsset/Effect/NS_WarningSector.NS_WarningSector'"
	));
	WarningEffect_Sector = WarningEffectObj_Sector.Object;
}

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

void ACubeHitActor::SpawnWarningCircle(float Size, float Time)
{
	if ( WarningEffect_Circle )
	{
		// 나이아가라 시스템 스폰
		UNiagaraComponent* NiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(),
			WarningEffect_Circle,   // WarningEffect 나이아가라 시스템
			GetActorLocation(),        // 생성할 위치
			GetActorRotation(),        // 생성할 회전 값
			FVector(1.0f)    // 기본 크기
		);

		if ( NiagaraComp )
		{
			// 파라미터 변경
			NiagaraComp->SetVariableFloat(FName("User.Time"), Time);  // Float 파라미터 설정
			NiagaraComp->SetVariableFloat(FName("User.Size"), Size);   // Vector 파라미터 설정

			// 일정 시간 후에 나이아가라 시스템 제거
			FTimerHandle TimerHandle;
			GetWorld()->GetTimerManager().SetTimer(
				TimerHandle,
				[ NiagaraComp ] ()
				{
				if ( NiagaraComp )
				{
					NiagaraComp->DestroyComponent(); // 나이아가라 컴포넌트 제거
				}
				},
				Time,   // Time 초 후에 실행
					false   // 반복하지 않음
					);
		}
	}
}

void ACubeHitActor::SpawnWarningSquare(float XOffset, float SizeX, float SizeY, float Time)
{
	if ( WarningEffect_Square )
	{
		// 나이아가라 시스템 스폰
		UNiagaraComponent* NiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(),
			WarningEffect_Square,   // WarningEffect 나이아가라 시스템
			GetActorLocation() + GetActorForwardVector() * XOffset,  // 생성할 위치
			GetActorRotation(),        // 생성할 회전 값
			FVector(1.0f)    // 기본 크기
		);
		NiagaraComp->SetWorldRotation(GetActorRotation());

		if ( NiagaraComp )
		{
			// 파라미터 변경
			NiagaraComp->SetVariableFloat(FName("User.Time"), Time);  // Float 파라미터 설정
			NiagaraComp->SetVariableFloat(FName("User.SizeX"), SizeX);   // Vector 파라미터 설정
			NiagaraComp->SetVariableFloat(FName("User.SizeY"), SizeY);   // Vector 파라미터 설정

			// 일정 시간 후에 나이아가라 시스템 제거
			FTimerHandle TimerHandle;
			GetWorld()->GetTimerManager().SetTimer(
				TimerHandle,
				[ NiagaraComp ] ()
				{
					if ( NiagaraComp )
					{
						NiagaraComp->DestroyComponent(); // 나이아가라 컴포넌트 제거
					}
				},
				Time,   // Time 초 후에 실행
					false   // 반복하지 않음
					);
		}
	}
}

void ACubeHitActor::SpawnWarningSector(float Size, float Angle, float Time)
{
	if ( WarningEffect_Sector )
	{
		// 나이아가라 시스템 스폰
		UNiagaraComponent* NiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(),
			WarningEffect_Sector,   // WarningEffect 나이아가라 시스템
			GetActorLocation(),        // 생성할 위치
			GetActorRotation(),// + FRotator(0, 0, Angle / 2),       // 생성할 회전 값
			FVector(1.0f)    // 기본 크기
		);

		if ( NiagaraComp )
		{
			// 파라미터 변경
			NiagaraComp->SetVariableFloat(FName("User.Time"), Time);  // Float 파라미터 설정
			NiagaraComp->SetVariableFloat(FName("User.Size"), Size);   // Vector 파라미터 설정
			NiagaraComp->SetVariableFloat(FName("User.Angle"), Angle);   // Vector 파라미터 설정

			// 일정 시간 후에 나이아가라 시스템 제거
			FTimerHandle TimerHandle;
			GetWorld()->GetTimerManager().SetTimer(
				TimerHandle,
				[ NiagaraComp ] ()
				{
					if ( NiagaraComp )
					{
						NiagaraComp->DestroyComponent(); // 나이아가라 컴포넌트 제거
					}
				},
				Time,   // Time 초 후에 실행
					false   // 반복하지 않음
					);
		}
	}
}
