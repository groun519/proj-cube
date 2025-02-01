// Fill out your copyright notice in the Description page of Project Settings.


#include "CubeActiveSkill.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

#include "P_Cube/Actor/CubeProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "P_Cube/Actor/CubeHitbox.h"

#include "P_Cube/Character/CubeCharacterBase.h"

#include "P_Cube/Interaction/CombatInterface.h"
#include "P_Cube/AbilitySystem/CubeAbilitySystemLibrary.h"

void UCubeActiveSkill::StoreMouseDataInfo(const FHitResult& HitResult)
{
	if ( HitResult.bBlockingHit )
	{
		MouseHitLocation = HitResult.ImpactPoint;
		MouseHitActor = HitResult.GetActor();
	}
	else
	{
		CancelAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true);
	}
}

void UCubeActiveSkill::StoreOwnerVariables()
{
	if ( CurrentActorInfo )
	{
		OwnerPlayerController = CurrentActorInfo->PlayerController.Get();
		OwnerCharacter = Cast<ACharacter>(CurrentActorInfo->AvatarActor);
	}
}

void UCubeActiveSkill::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UCubeActiveSkill::ApplyDamageByName(AActor* Target, FName DamageName) const
{
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if ( !bIsServer ) return;

	FDamageEffectParams DamageEffectParams = MakeDamageEffectParamsFromClassDefaults(Target, DamageName);

	FString DamageNameString = DamageName.ToString();
	bool bDamageTypeIsHeal = false;
	if ( DamageNameString.Contains("Heal") )
	{
		bDamageTypeIsHeal = true;
	}

	if ( DamageEffectParams.SourceAbilitySystemComponent == nullptr ) return;

	AActor* SourceAvatarActor = DamageEffectParams.SourceAbilitySystemComponent->GetAvatarActor();
	//if ( SourceAvatarActor == Target ) return; 자기자신 안때리게 하는건데 굳이?
	if ( !UCubeAbilitySystemLibrary::IsNotFriend(SourceAvatarActor, Target) && !bDamageTypeIsHeal ) return;

	AActor* AvatarActor = GetAvatarActorFromActorInfo();

	if ( ApplyCrowdControll(DamageEffectParams, Target, AvatarActor) )
	{
		UCubeAbilitySystemLibrary::ApplyDamageEffect(DamageEffectParams); // damage 이펙트 적용.
	}

	for ( TSubclassOf<UGameplayEffect> Effect : DamageInfoMap[ DamageName ].Effects )
	{
		if ( Effect && DamageEffectParams.TargetAbilitySystemComponent )
		{
			FGameplayEffectSpecHandle EffectSpecHandle = MakeOutgoingGameplayEffectSpec(Effect, 1.0f);
			DamageEffectParams.TargetAbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*EffectSpecHandle.Data.Get(), DamageEffectParams.TargetAbilitySystemComponent);
		}
	}
}

void UCubeActiveSkill::SpawnProjectile(const FName ProjectileName, const FName DamageName, const FVector& ProjectileTargetLocation, const FGameplayTag& SocketTag, bool bOverridePitch, float PitchOverride, bool bOverrideYaw, float YawOverride, AActor* InstigatorPlayer, bool bIsOnlyAttackTargetActor, AActor* TargetActor, FMultipleProjectilesFeacher MultipleProjectilesFeacher, FHomingFeacher HomingFeacher) // 투사체 생성
{
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!bIsServer) return;

	const FVector SocketLocation = ICombatInterface::Execute_GetCombatSocketLocation(
		GetAvatarActorFromActorInfo(),
		SocketTag); // 투사체 생성 소켓(WeaponHandSocket)의 위치정보를 받아옴. <- 소켓 위치에서 시작되기 때문에, 돌진형 스킬을 사용하거나, 마우스 위치를 캐릭터와 소켓 사이에 둘 경우 반대 방향으로 투사체를 발사하는 경우가 발생함. 나중에 소켓 기준이 아니라 캐릭터 기준으로 변경하는 게 좋을 듯.
	FRotator Rotation = (ProjectileTargetLocation - SocketLocation).Rotation();
	if (bOverridePitch)
	{
		Rotation.Pitch = PitchOverride;
	}
	if (bOverrideYaw)
	{
		Rotation.Yaw += YawOverride;
	}

	const FVector Forward = Rotation.Vector();

	/*FTransform SpawnTransform;
	SpawnTransform.SetLocation(SocketLocation);
	SpawnTransform.SetRotation(Rotation.Quaternion());*/

	/*ACubeProjectile* Projectile = GetWorld()->SpawnActorDeferred<ACubeProjectile>(
		ProjectileClassMap[ ProjectileName ],
		SpawnTransform,
		GetOwningActorFromActorInfo(),
		Cast<APawn>(GetOwningActorFromActorInfo()),
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn);*/

	TArray<FRotator> Rotations;

	if ( MultipleProjectilesFeacher.bUseMultipleProjectilesFeacher )
	{
		const int32 EffectiveNumProjectiles = 
			FMath::Min(
				MultipleProjectilesFeacher.NumProjectiles + ( GetAbilityLevel() - 1 ) * ( MultipleProjectilesFeacher.UpgradeProjectiles ),
				MultipleProjectilesFeacher.MaxNumProjectiles
			);
		Rotations = UCubeAbilitySystemLibrary::EvenlySpacedRotators(Forward, FVector::UpVector, MultipleProjectilesFeacher.ProjectileSpread, EffectiveNumProjectiles);
	}
	else
	{
		Rotations.Add(Rotation);
	}

	for ( const FRotator& Rot : Rotations ) // 발사할 투사체 개수만큼 반복 (Rotation: 각 투사체의 발사방향)
	{
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(SocketLocation);
		SpawnTransform.SetRotation(Rot.Quaternion());

		/** 투사체 객체 설정 **/
		ACubeProjectile* Projectile = GetWorld()->SpawnActorDeferred<ACubeProjectile>(
		ProjectileClassMap[ ProjectileName ],
		SpawnTransform,
		GetOwningActorFromActorInfo(),
		Cast<APawn>(GetOwningActorFromActorInfo()),
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

		if ( InstigatorPlayer )
		{
			Projectile->InstigatorPlayer = InstigatorPlayer;
		}
		if ( bIsOnlyAttackTargetActor )
		{
			Projectile->bIsAttackOnlyTarget = bIsOnlyAttackTargetActor;
		}
		if ( TargetActor )
		{
			Projectile->TargetActor = TargetActor;
		}

		Projectile->DamageEffectParams = MakeDamageEffectParamsFromClassDefaults(TargetActor, DamageName);
		FString DamageNameString = DamageName.ToString();
		if ( DamageNameString.Contains("Heal") )
		{
			Projectile->bDamageTypeIsHeal = true;
		}

		if ( HomingFeacher.HomingTarget && HomingFeacher.HomingTarget->Implements<UCombatInterface>() )
		{
			Projectile->ProjectileMovement->HomingTargetComponent = HomingFeacher.HomingTarget->GetRootComponent();
		}
		else
		{
			Projectile->HomingTargetSceneComponent = NewObject<USceneComponent>(USceneComponent::StaticClass());
			Projectile->HomingTargetSceneComponent->SetWorldLocation(ProjectileTargetLocation);
			Projectile->ProjectileMovement->HomingTargetComponent = Projectile->HomingTargetSceneComponent;
		}
		Projectile->ProjectileMovement->HomingAccelerationMagnitude =
			FMath::FRandRange(
				HomingFeacher.HomingAccelerationMin,
				HomingFeacher.HomingAccelerationMax
			);
		Projectile->ProjectileMovement->bIsHomingProjectile = HomingFeacher.bUseHomingFeacher;

		/** GA 접근 가능하게 할당 **/
		Projectile->LinkedAbility = this;

		/** 투사체 스폰 **/
		Projectile->FinishSpawning(SpawnTransform);
	}

	///** 피해량 관련 **/
	//Projectile->DamageEffectParams = MakeDamageEffectParamsFromClassDefaults(TargetActor, DamageName);
	//FString DamageNameString = DamageName.ToString();
	//if ( DamageNameString.Contains("Heal") )
	//{
	//	Projectile->bDamageTypeIsHeal = true;
	//}

	/** GA 접근 가능하게 할당 **/
	/*Projectile->LinkedAbility = this;*/
	
	/** 투사체 스폰 **/
	/*Projectile->FinishSpawning(SpawnTransform);*/
}


void UCubeActiveSkill::SpawnHitBox(const FName HitboxName, const FName DamageName, const FTransform& HitboxTransform, const bool bOnlyPlayer, AActor* InstigatorPlayer, AActor* TargetActor)
{
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if ( !bIsServer ) return;

	FTransform SpawnTransform;
	SpawnTransform.SetLocation(HitboxTransform.GetLocation());
	SpawnTransform.SetRotation(HitboxTransform.GetRotation());
	SpawnTransform.SetScale3D(HitboxTransform.GetScale3D());

	ACubeHitbox* Hitbox = GetWorld()->SpawnActorDeferred<ACubeHitbox>(
		HitboxClassMap[ HitboxName ],
		SpawnTransform,
		GetOwningActorFromActorInfo(),
		Cast<APawn>(GetOwningActorFromActorInfo()),	
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

	if ( InstigatorPlayer )
	{
		Hitbox->InstigatorPlayer = InstigatorPlayer;
	}
	if ( TargetActor )
	{
		Hitbox->TargetActor = TargetActor;
	}
	if ( bOnlyPlayer )
	{
		Hitbox->bOnlyPlayer = true;
	}

	Hitbox->DamageEffectParams = MakeDamageEffectParamsFromClassDefaults(TargetActor, DamageName);

	FString DamageNameString = DamageName.ToString();
	if ( DamageNameString.Contains("Heal"))
	{
		Hitbox->bDamageTypeIsHeal = true;
	}

	Hitbox->LinkedAbility = this;

	Hitbox->FinishSpawning(SpawnTransform);
}

TArray<AActor*> UCubeActiveSkill::FindNearestActorsByTag(const FName TagName, const FVector Location, const float Radius, const int32 findingPlayers, const bool bDrawDebugSphere)
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
				ACubeCharacterBase* CubeCharacter = Cast<ACubeCharacterBase>(OverlappedActor);
				if ( CubeCharacter && !CubeCharacter->Execute_IsDead(OverlappedActor) )
				{
					NearbyPlayers.Add(CubeCharacter);
				}
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
