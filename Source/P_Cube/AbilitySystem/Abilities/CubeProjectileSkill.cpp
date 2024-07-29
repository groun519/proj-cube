// Fill out your copyright notice in the Description page of Project Settings.


#include "CubeProjectileSkill.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "P_Cube/Actor/CubeProjectile.h"
#include "P_Cube/Interaction/CombatInterface.h"

void UCubeProjectileSkill::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UCubeProjectileSkill::SpawnProjectile(const FVector& ProjectileTargetLocation) // 투사체 생성
{
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!bIsServer) return;

	ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo());
	if (CombatInterface)
	{
		const FVector SocketLocation = CombatInterface->GetCombatSocketLocation(); // 투사체 생성 소켓(WeaponHandSocket)의 위치정보를 받아옴. <- 소켓 위치에서 시작되기 때문에, 돌진형 스킬을 사용하거나, 마우스 위치를 캐릭터와 소켓 사이에 둘 경우 반대 방향으로 투사체를 발사하는 경우가 발생함. 나중에 소켓 기준이 아니라 캐릭터 기준으로 변경하는 게 좋을 듯.
		FRotator Rotation = (ProjectileTargetLocation - SocketLocation).Rotation();
		//Rotation.Pitch = 0.f;

		FTransform SpawnTransform;
		SpawnTransform.SetLocation(SocketLocation);
		SpawnTransform.SetRotation(Rotation.Quaternion());

		ACubeProjectile* Projectile = GetWorld()->SpawnActorDeferred<ACubeProjectile>(
			ProjectileClass,
			SpawnTransform,
			GetOwningActorFromActorInfo(),
			Cast<APawn>(GetOwningActorFromActorInfo()),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

		const UAbilitySystemComponent* SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo());
		const FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(DamageEffectClass, GetAbilityLevel(), SourceASC->MakeEffectContext());
		Projectile->DamageEffectSpecHandle = SpecHandle;

		Projectile->FinishSpawning(SpawnTransform);
	}
}
