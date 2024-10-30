// Fill out your copyright notice in the Description page of Project Settings.


#include "CubeActiveSkill.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

#include "P_Cube/Actor/CubeProjectile.h"
#include "P_Cube/Actor/CubeHitbox.h"

#include "P_Cube/Interaction/CombatInterface.h"
#include "P_Cube/CubeGameplayTags.h"

void UCubeActiveSkill::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UCubeActiveSkill::SpawnProjectile(const FName ProjectileName, const FName DamageName, const FVector& ProjectileTargetLocation, const FGameplayTag& SocketTag, bool bOverridePitch, float PitchOverride, bool bOverrideYaw, float YawOverride, AActor* InstigatorPlayer, bool bIsOnlyAttackTargetActor, AActor* TargetActor) // 투사체 생성
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

	FTransform SpawnTransform;
	SpawnTransform.SetLocation(SocketLocation);
	SpawnTransform.SetRotation(Rotation.Quaternion());

	ACubeProjectile* Projectile = GetWorld()->SpawnActorDeferred<ACubeProjectile>(
		ProjectileClassMap[ ProjectileName ],
		SpawnTransform,
		GetOwningActorFromActorInfo(),
		Cast<APawn>(GetOwningActorFromActorInfo()),
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

	if (InstigatorPlayer)
	{
		Projectile->InstigatorPlayer = InstigatorPlayer;
	}
	if (bIsOnlyAttackTargetActor)
	{
		Projectile->bIsAttackOnlyTarget = bIsOnlyAttackTargetActor;
	}
	if (TargetActor)
	{
		Projectile->TargetActor = TargetActor;
	}

	Projectile->DamageEffectParams = MakeDamageEffectParamsFromClassDefaults(TargetActor, DamageName);

	Projectile->FinishSpawning(SpawnTransform);
}

void UCubeActiveSkill::SpawnHitBox(const FName HitboxName, const FName DamageName, const FTransform& HitboxTransform, AActor* InstigatorPlayer, AActor* TargetActor)
{
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if ( !bIsServer ) return;

	FTransform SpawnTransform;
	SpawnTransform.SetLocation(HitboxTransform.GetLocation());
	SpawnTransform.SetRotation(HitboxTransform.GetRotation());

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

	Hitbox->DamageEffectParams = MakeDamageEffectParamsFromClassDefaults(TargetActor, DamageName);

	Hitbox->FinishSpawning(SpawnTransform);
}
