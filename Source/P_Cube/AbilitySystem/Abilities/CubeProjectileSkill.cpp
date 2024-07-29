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

void UCubeProjectileSkill::SpawnProjectile(const FVector& ProjectileTargetLocation) // ����ü ����
{
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!bIsServer) return;

	ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo());
	if (CombatInterface)
	{
		const FVector SocketLocation = CombatInterface->GetCombatSocketLocation(); // ����ü ���� ����(WeaponHandSocket)�� ��ġ������ �޾ƿ�. <- ���� ��ġ���� ���۵Ǳ� ������, ������ ��ų�� ����ϰų�, ���콺 ��ġ�� ĳ���Ϳ� ���� ���̿� �� ��� �ݴ� �������� ����ü�� �߻��ϴ� ��찡 �߻���. ���߿� ���� ������ �ƴ϶� ĳ���� �������� �����ϴ� �� ���� ��.
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
