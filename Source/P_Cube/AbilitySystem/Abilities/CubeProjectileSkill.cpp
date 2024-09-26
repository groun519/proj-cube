// Fill out your copyright notice in the Description page of Project Settings.


#include "CubeProjectileSkill.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "P_Cube/Actor/CubeProjectile.h"
#include "P_Cube/Interaction/CombatInterface.h"
#include "P_Cube/CubeGameplayTags.h"

void UCubeProjectileSkill::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UCubeProjectileSkill::SpawnProjectile(const FVector& ProjectileTargetLocation, const FGameplayTag& SocketTag, bool bOverridePitch, float PitchOverride) // ����ü ����
{
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!bIsServer) return;

	const FVector SocketLocation = ICombatInterface::Execute_GetCombatSocketLocation(
		GetAvatarActorFromActorInfo(),
		SocketTag); // ����ü ���� ����(WeaponHandSocket)�� ��ġ������ �޾ƿ�. <- ���� ��ġ���� ���۵Ǳ� ������, ������ ��ų�� ����ϰų�, ���콺 ��ġ�� ĳ���Ϳ� ���� ���̿� �� ��� �ݴ� �������� ����ü�� �߻��ϴ� ��찡 �߻���. ���߿� ���� ������ �ƴ϶� ĳ���� �������� �����ϴ� �� ���� ��.
	FRotator Rotation = (ProjectileTargetLocation - SocketLocation).Rotation();
	if (bOverridePitch)
	{
		Rotation.Pitch = PitchOverride;
	}

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
	// EffectContextHandle ����
	FGameplayEffectContextHandle EffectContextHandle = SourceASC->MakeEffectContext();
	EffectContextHandle.SetAbility(this);
	EffectContextHandle.AddSourceObject(Projectile);
	TArray<TWeakObjectPtr<AActor>> Actors;
	Actors.Add(Projectile);
	EffectContextHandle.AddActors(Actors);
	FHitResult HitResult;
	HitResult.Location = ProjectileTargetLocation;
	EffectContextHandle.AddHitResult(HitResult); //

	const FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(DamageEffectClass, GetAbilityLevel(), EffectContextHandle);

	const FCubeGameplayTags GameplayTags = FCubeGameplayTags::Get();

	for (auto& Pair : DamageTypes)
	{
		const float ScaledDamage = Pair.Value.GetValueAtLevel(GetAbilityLevel());
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, Pair.Key, ScaledDamage);
	}

	Projectile->DamageEffectSpecHandle = SpecHandle;

	Projectile->FinishSpawning(SpawnTransform);
}
