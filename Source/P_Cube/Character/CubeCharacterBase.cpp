// Fill out your copyright notice in the Description page of Project Settings.


#include "CubeCharacterBase.h"
#include "AbilitySystemComponent.h"
#include "P_Cube/CubeGameplayTags.h"
#include "P_Cube/AbilitySystem/CubeAbilitySystemComponent.h"
#include "P_Cube/P_Cube.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "P_Cube/AbilitySystem/CubeAttributeSet.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"

ACubeCharacterBase::ACubeCharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;

	
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore); // 캡슐 콜리전 카메라 충돌 무시하게 설정. (플레이어에 카메라가 영향 받는 것을 방지)
	GetCapsuleComponent()->SetGenerateOverlapEvents(false); // 충돌 이벤트 가능하게 설정.
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore); // 메쉬 카메라 충돌 무시 설정.
	GetMesh()->SetCollisionResponseToChannel(ECC_Projectile, ECR_Overlap); // 투사체 접촉 판정을 Overlap으로 설정.
	GetMesh()->SetCollisionResponseToChannel(ECC_Hitbox, ECR_Overlap); // 투사체 접촉 판정을 Overlap으로 설정.
	GetMesh()->SetGenerateOverlapEvents(true); // 충돌 이벤트 가능하게 설정.

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	Weapon->SetupAttachment(GetMesh(), FName("WeaponHandSocket"));
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	GetCharacterMovement()->SetIsReplicated(true);
	bReplicates = true;
	bAlwaysRelevant = true;
	//GetCharacterMovement()->bUseControllerDesiredRotation = false;
	//GetCharacterMovement()->bOrientRotationToMovement = true;

	/*StunDebuffComponent = CreateDefaultSubobject<UDebuffNiagaraComponent>("StunDebuffComponent");
	StunDebuffComponent->SetupAttachment(GetRootComponent());
	StunDebuffComponent->DebuffTag = GameplayTags.Debuff_Stun;*/
	
	EffectAttachComponent = CreateDefaultSubobject<USceneComponent>("EffectAttachPoint");
	EffectAttachComponent->SetupAttachment(GetRootComponent());
}

void ACubeCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	EffectAttachComponent->SetWorldRotation(FRotator::ZeroRotator);
}

void ACubeCharacterBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACubeCharacterBase, bIsStunned);
	DOREPLIFETIME_CONDITION(ACubeCharacterBase, ReplicatedMaxWalkSpeed, COND_None);
}

UAbilitySystemComponent* ACubeCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UAnimMontage* ACubeCharacterBase::GetHitReactMontage_Implementation()
{
	return HitReactMontage;
}

void ACubeCharacterBase::Die()
{
	Weapon->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true)); // 부모에서 분리
	MulticastHandleDeath();
}

FOnDeathSignature& ACubeCharacterBase::GetOnDeathDelegate()
{
	return OnDeathDelegate;
}

void ACubeCharacterBase::MulticastHandleDeath_Implementation() // 래그돌  함수
{
	UGameplayStatics::PlaySoundAtLocation(this, DeathSound, GetActorLocation(), GetActorRotation());

	Weapon->SetSimulatePhysics(true);
	Weapon->SetEnableGravity(true);
	Weapon->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);

	GetMesh()->SetSimulatePhysics(true); // 캐릭터 메쉬 물리 시뮬레이션 활성화
	GetMesh()->SetEnableGravity(true); // 캐릭터 메쉬 중력 활성화
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetMesh()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Dissolve(); // 사라지는 이펙트
	bDead = true; // 이거 추가한 부분 강의 다시봐야함.
	OnDeathDelegate.Broadcast(this);
}

void ACubeCharacterBase::OnRep_Stunned()
{

}

void ACubeCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	if ( const UCubeAttributeSet* CubeAS = Cast<UCubeAttributeSet>(AttributeSet) )
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(CubeAS->GetMovementSpeedIncreaseRateAttribute()).AddLambda(
			[ this ] (const FOnAttributeChangeData& Data)
			{
				const float NewSpeed = BaseSpeed * Data.NewValue;
				//ReplicatedMaxWalkSpeed = NewSpeed;

				//Server_UpdateMovementSpeed(GetCharacterMovement());

				/*UE_LOG(LogTemp, Log, TEXT("MovementSpeed Changed (Server), NewSpeed : %f, RepSpeed : %f"), NewSpeed, ReplicatedMaxWalkSpeed);
				UE_LOG(LogTemp, Log, TEXT("BaseSpeed : %f"), BaseSpeed);
				UE_LOG(LogTemp, Log, TEXT("NewValue : %f"), Data.NewValue);
				UE_LOG(LogTemp, Log, TEXT("NewSpeed : %f"), NewSpeed);
				UE_LOG(LogTemp, Log, TEXT("RepSpeed : %f"), ReplicatedMaxWalkSpeed);*/

				/*if ( HasAuthority() )
				{
					UE_LOG(LogTemp, Log, TEXT("Server MovementSpeed Changed"));
					UpdateMovementSpeed(GetCharacterMovement());
				}
				else
				{
					UE_LOG(LogTemp, Log, TEXT("Client MovementSpeed Changed"));
					Server_UpdateMovementSpeed(GetCharacterMovement());
				}*/
			}
		);
	}

	if (WeaponTag.IsValid())
	{
		Execute_SetBaseWeapon(this, WeaponTag);
	}
}

FVector ACubeCharacterBase::GetCombatSocketLocation_Implementation(const FGameplayTag& MontageTag)
{
	const FCubeGameplayTags& GameplayTags = FCubeGameplayTags::Get();
	if (MontageTag.MatchesTagExact(GameplayTags.CombatSocket_ActorLocation))
	{
		return GetActorLocation();
	}
	if (MontageTag.MatchesTagExact(GameplayTags.CombatSocket_Weapon) && IsValid(Weapon))
	{
		return Weapon->GetSocketLocation(WeaponTipSocketName);
	}
	if (MontageTag.MatchesTagExact(GameplayTags.CombatSocket_LeftHand))
	{
		return GetMesh()->GetSocketLocation(LeftHandSocketName);
	}
	if (MontageTag.MatchesTagExact(GameplayTags.CombatSocket_RightHand))
	{
		return GetMesh()->GetSocketLocation(RightHandSocketName);
	}
	return FVector();
}

bool ACubeCharacterBase::IsDead_Implementation() const
{
	return bDead;
}

AActor* ACubeCharacterBase::GetAvatar_Implementation()
{
	return this;
}

TArray<FTaggedMontage> ACubeCharacterBase::GetAttackMontages_Implementation()
{
	return AttackMontages;
}

UNiagaraSystem* ACubeCharacterBase::GetBloodEffect_Implementation()
{
	return BloodEffect;
}

FTaggedMontage ACubeCharacterBase::GetTaggedMontageByTag_Implementation(const FGameplayTag& MontageTag)
{
	for (FTaggedMontage TaggedMontage : AttackMontages)
	{
		if (TaggedMontage.MontageTag == MontageTag)
		{
			return TaggedMontage;
		}
	}
	return FTaggedMontage();
}

int32 ACubeCharacterBase::GetMinionCount_Implementation()
{
	return MinionCount;
}

void ACubeCharacterBase::IncremenetMinionCount_Implementation(int32 Amount)
{
	MinionCount += Amount;
}

ECharacterClass ACubeCharacterBase::GetCharacterClass_Implementation()
{
	return CharacterClass;
}

USkeletalMeshComponent* ACubeCharacterBase::GetWeapon_Implementation()
{
	return Weapon;
}

void ACubeCharacterBase::SetBaseWeapon_Implementation(FGameplayTag NewWeaponTag)
{
	if ( Weapon )
	{
		BaseWeaponMesh = Weapon->SkeletalMesh;

		if ( WeaponInfo->FindWeaponInfoForTag(NewWeaponTag).WeaponMesh )
		{
			FWeaponInformation Info = WeaponInfo->FindWeaponInfoForTag(NewWeaponTag);
			Weapon->SetSkeletalMesh(Info.WeaponMesh);
			BaseWeaponOffset = Info.WeaponOffset;
			Weapon->SetRelativeTransform(Info.WeaponOffset);
		}
	}
}

void ACubeCharacterBase::ChangeWeapon_Implementation(FGameplayTag NewWeaponTag)
{
	if ( Weapon )
	{
		if ( WeaponInfo->FindWeaponInfoForTag(NewWeaponTag).WeaponMesh )
		{
			const FCubeGameplayTags& GameplayTags = FCubeGameplayTags::Get();
			FGameplayTag AttackInputTag = GameplayTags.InputTag_RMB;

			FWeaponInformation Info = WeaponInfo->FindWeaponInfoForTag(NewWeaponTag);

			Server_SetWeaponMesh(Info.WeaponMesh, Info.WeaponOffset);

			UCubeAbilitySystemComponent* CubeASC = CastChecked<UCubeAbilitySystemComponent>(AbilitySystemComponent);
			if ( CubeASC /*&& AttackAbility*/ )
			{
				//CubeASC->RemoveCharacterAbility(AttackAbility);
				CubeASC->ClearAbilitiesOfSlot(AttackInputTag);
				//AttackAbility = Info.AttackAbility;
				//CubeASC->AddCharacterAbility(AttackAbility);
				CubeASC->ServerAddAttackAbility(NewWeaponTag);
				//CubeASC->ServerEquipAbility(NewWeaponTag, AttackInputTag, true);

				/*CubeASC->SetNumericAttributeBase(UCubeAttributeSet::GetRangeAttribute(), Info.Range);
				CubeASC->SetNumericAttributeBase(UCubeAttributeSet::GetMovementSpeedAttribute(), 160.0f * Info.MovementSpeed_Coef);
				CubeASC->SetNumericAttributeBase(UCubeAttributeSet::GetAttackSpeedAttribute(), 1.0f * Info.AttackSpeed_Coef);*/
			
				Server_ApplyWeaponEffect(Info.Range, Info.MovementSpeed_Coef, Info.AttackSpeed_Coef);
			}
			//else if ( CubeASC && !AttackAbility )
			//{
			//	CubeASC->AddCharacterAbility(Info.AttackAbility);
			//	//CubeASC->ServerEquipAbility(Info.AttackAbilityTag, AttackInputTag, true);
			//}
		}
	}
}

void ACubeCharacterBase::ResetWeapon_Implementation()
{
	if ( Weapon && BaseWeaponMesh )
	{
		Weapon->SetSkeletalMesh(BaseWeaponMesh);
		Weapon->SetRelativeTransform(BaseWeaponOffset);
	}
}

FOnASCRegistered ACubeCharacterBase::GetOnASCRegisteredDelegate()
{
	return OnAscRegistered;
}

void ACubeCharacterBase::StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	bIsStunned = NewCount > 0;
	GetCharacterMovement()->MaxWalkSpeed = bIsStunned ? 0.f : ReplicatedMaxWalkSpeed;
}

void ACubeCharacterBase::OnRep_MaxWalkSpeed()
{
	if ( GetCharacterMovement() )
	{
		UE_LOG(LogTemp, Warning, TEXT("[OnRep] 이동 속도 변경: %f"), ReplicatedMaxWalkSpeed);
		GetCharacterMovement()->MaxWalkSpeed = ReplicatedMaxWalkSpeed;
	}
}

void ACubeCharacterBase::UpdateMovementSpeed(UCharacterMovementComponent* CharacterMovementComp)
{
	UE_LOG(LogTemp, Log, TEXT("UpdateMovementSpeed Linked"));
	CharacterMovementComp->MaxWalkSpeed = ReplicatedMaxWalkSpeed;
}

void ACubeCharacterBase::Server_UpdateMovementSpeed_Implementation(UCharacterMovementComponent* CharacterMovementComp)
{
	UE_LOG(LogTemp, Log, TEXT("Server_UpdateMovementSpeed Linked"));
	UpdateMovementSpeed(CharacterMovementComp);
}

void ACubeCharacterBase::Multicast_UpdateMovementSpeed_Implementation(float NewSpeed)
{
	if ( GetCharacterMovement() )
	{
		GetCharacterMovement()->MaxWalkSpeed = NewSpeed;
	}
}

void ACubeCharacterBase::InitAbilityActorInfo()
{
}

void ACubeCharacterBase::ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level) const
{
	check(IsValid(GetAbilitySystemComponent()));
	check(GameplayEffectClass);
	FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	ContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(GameplayEffectClass, Level, ContextHandle);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), GetAbilitySystemComponent());
}

void ACubeCharacterBase::InitializeDefaultAttributes() const
{
	ApplyEffectToSelf(DefaultPrimaryAttributes, 1.f);
	ApplyEffectToSelf(DefaultSecondaryAttributes, 1.f);
	ApplyEffectToSelf(DefaultVitalAttributes, 1.f);
}

void ACubeCharacterBase::AddCharacterAbilities()
{
	UCubeAbilitySystemComponent* CubeASC = CastChecked<UCubeAbilitySystemComponent>(AbilitySystemComponent);
	if (!HasAuthority()) return;

	//CubeASC->AddCharacterAbilities(StartupAbilities);
	CubeASC->AddCharacterPassiveAbilities(StartupPassiveAbilities);
	

	if ( CharacterClass == ECharacterClass::Warrior )
	{
		CubeASC->AddCharacterAbilities(StartupAbilitiesMap[ "Warrior" ].Abilities);
	}
	else if ( CharacterClass == ECharacterClass::Ranger )
	{
		CubeASC->AddCharacterAbilities(StartupAbilitiesMap[ "Ranger" ].Abilities);
	}
	else if ( CharacterClass == ECharacterClass::Wizard )
	{
		CubeASC->AddCharacterAbilities(StartupAbilitiesMap[ "Wizard" ].Abilities);
	}
	else if ( CharacterClass == ECharacterClass::Healer )
	{
		CubeASC->AddCharacterAbilities(StartupAbilitiesMap[ "Healer" ].Abilities);
	}
	else CubeASC->AddCharacterAbilities(StartupAbilitiesMap[ "Test" ].Abilities);


}

void ACubeCharacterBase::Dissolve()
{
	if (IsValid(DissolveMaterialInstance)) // 캐릭터 메쉬 디졸브 이펙트 적용
	{
		UMaterialInstanceDynamic* DynamicMatInst = UMaterialInstanceDynamic::Create(DissolveMaterialInstance, this);
		GetMesh()->SetMaterial(0, DynamicMatInst);
		StartDissolveTimeline(DynamicMatInst);
	}
	if (IsValid(WeaponDissolveMaterialInstance)) // 무기 메쉬 디졸브 이펙트 적용
	{
		UMaterialInstanceDynamic* DynamicMatInst = UMaterialInstanceDynamic::Create(WeaponDissolveMaterialInstance, this);
		Weapon->SetMaterial(0, DynamicMatInst);
		StartWeaponDissolveTimeline(DynamicMatInst);
	}
}

void ACubeCharacterBase::SetWeaponTag(FGameplayTag NewTag)
{
	if (WeaponTag.IsValid())
	{
		WeaponTag = NewTag;
		Execute_SetBaseWeapon(this, NewTag); // TODO : if baseweapon is not useable, remove this
		Execute_ChangeWeapon(this, NewTag);
	}
}

void ACubeCharacterBase::Server_ApplyWeaponEffect_Implementation(float Range, float MovementSpeed_Coef, float AttackSpeed_Coef)
{
	if ( HasAuthority() )
	{
		Multicast_ApplyWeaponEffect(Range, MovementSpeed_Coef, AttackSpeed_Coef);
	}
}

void ACubeCharacterBase::Multicast_ApplyWeaponEffect_Implementation(float Range, float MovementSpeed_Coef, float AttackSpeed_Coef)
{
	if ( AbilitySystemComponent )
	{
		// 새로운 GameplayEffect 동적 생성
		UGameplayEffect* NewEffect = NewObject<UGameplayEffect>(GetTransientPackage(), FName("Dynamic_WeaponStatEffect"));
		NewEffect->DurationPolicy = EGameplayEffectDurationType::Instant;

		FGameplayModifierInfo RangeModifier;
		RangeModifier.Attribute = UCubeAttributeSet::GetRangeAttribute(); // Range 속성 지정
		RangeModifier.ModifierOp = EGameplayModOp::Override; // 기존 값 덮어쓰기
		RangeModifier.ModifierMagnitude = FScalableFloat(Range);
		NewEffect->Modifiers.Add(RangeModifier);

		//FGameplayModifierInfo MovementSpeedModifier;
		//MovementSpeedModifier.Attribute = UCubeAttributeSet::GetMovementSpeedAttribute(); // Range 속성 지정
		//MovementSpeedModifier.ModifierOp = EGameplayModOp::Override; // 기존 값 덮어쓰기
		//MovementSpeedModifier.ModifierMagnitude = FScalableFloat(160.f * MovementSpeed_Coef);
		//NewEffect->Modifiers.Add(MovementSpeedModifier);

		FGameplayModifierInfo AttackSpeedModifier;
		AttackSpeedModifier.Attribute = UCubeAttributeSet::GetAttackSpeedAttribute(); // Range 속성 지정
		AttackSpeedModifier.ModifierOp = EGameplayModOp::Override; // 기존 값 덮어쓰기
		AttackSpeedModifier.ModifierMagnitude = FScalableFloat(1.0f * AttackSpeed_Coef);
		NewEffect->Modifiers.Add(AttackSpeedModifier);


		FGameplayEffectSpec Spec(NewEffect, AbilitySystemComponent->MakeEffectContext(), 1.f);
		AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(Spec);

		// GameplayEffectSpec 생성 후 적용
		/*FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(NewEffect->GetClass(), 1.f, AbilitySystemComponent->MakeEffectContext());

		if ( SpecHandle.IsValid() )
		{
			AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		}*/
	}
}

void ACubeCharacterBase::Server_SetWeaponMesh_Implementation(USkeletalMesh* NewMesh, FTransform NewTransform)
{
	if ( HasAuthority() )
	{
		Multicast_SetWeaponMesh(NewMesh, NewTransform);
	}
}

void ACubeCharacterBase::Multicast_SetWeaponMesh_Implementation(USkeletalMesh* NewMesh, FTransform NewTransform)
{
	if ( Weapon )
	{
		Weapon->SetSkeletalMesh(NewMesh);
		Weapon->SetRelativeTransform(NewTransform);
	}
}
