// Fill out your copyright notice in the Description page of Project Settings.


#include "CubeAttackAbility.h"
#include "P_Cube/Player/CubePlayerController.h"

#include "AIController.h"
#include "GameFramework/Character.h"
#include "NavigationSystem.h"

#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include <P_Cube/Character/CubeCharacter.h>
#include <P_Cube/Character/CubeCharacterBase.h>
#include "P_Cube/AbilitySystem/CubeAbilitySystemLibrary.h"
#include "P_Cube/AbilitySystem/CubeAttributeSet.h"

#include "TimerManager.h"
#include <AbilitySystemBlueprintLibrary.h>

UCubeAttackAbility::UCubeAttackAbility()
{
	bAttackLoopActive = false; 
}

//void UCubeAttackAbility::AttackCycle(AActor* AttackTarget, UAnimMontage* AttackMontage, float Range, float AttackSpeed, float PreDelay, float PostDelay)
//{
//	if ( !bAttackLoopActive ) return;
//	// Target is valid?
//	if ( !IsValid(AttackTarget) ) return;
//
//	PostDelay = FMath::Max(0.03f, PostDelay / AttackSpeed);
//
//	ACubeCharacter* Character = Cast<ACubeCharacter>(GetAvatarActorFromActorInfo());
//	if ( !Character ) return;
//	Character->AttackTarget = AttackTarget;
//
//	ACubeCharacterBase* TargetCharacter = Cast<ACubeCharacterBase>(AttackTarget);
//	if ( !TargetCharacter ) return;
//
//	// Calculate distance
//	float Distance = Character->GetDistanceTo(TargetCharacter);
//
//	if ( Distance < Range )
//	{
//		FVector TargetLoc = TargetCharacter->GetActorLocation();
//
//		if ( !TargetCharacter->ActorHasTag("Player") && !TargetCharacter->ActorHasTag("Enemy") )return;
//		//if ( TargetCharacter->Execute_IsDead(TargetCharacter) ) return;
//		if ( !PlayAttackMontage(AttackMontage, AttackSpeed * ( PreDelay + 0.1 )) ) return;
//	}
//	else
//	{
//		AAIController* Controller = Cast<AAIController>(Character->GetController());
//		if ( Controller )
//		{
//			Controller->MoveToActor(TargetCharacter, 0.f, true, true, false, 0, true);
//		}
//		Distance = Character->GetDistanceTo(TargetCharacter);
//	}
//}

void UCubeAttackAbility::CancelAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);

	if ( GetWorld() )
	{
		GetWorld()->GetTimerManager().ClearTimer(TickTimerHandle);
	}
	
	ACubeCharacter* Character = Cast<ACubeCharacter>(GetAvatarActorFromActorInfo());
	if ( IsValid(Character) )
	{
		Character->AttackTarget = nullptr;
	}
}

void UCubeAttackAbility::StartTickTimer(
		const FOnTickEvent& TickEvent,
		AActor* NewAttackTarget)
{
	ACubeCharacter* Character = Cast<ACubeCharacter>(GetAvatarActorFromActorInfo());
	if ( !IsValid(Character) ) return;

	Character->AttackTarget = NewAttackTarget;

	StoredTickEvent = TickEvent;

	GetWorld()->GetTimerManager().SetTimer(
		TickTimerHandle,
		this,
		&UCubeAttackAbility::OnTick,
		0.1f,  
		true    
	);
}

void UCubeAttackAbility::OnTick()
{
	//UE_LOG(LogTemp, Log, TEXT("어빌리티 Tick 중..."));

	ACubeCharacter* Character = Cast<ACubeCharacter>(GetAvatarActorFromActorInfo());
	if (!IsValid(Character)) return;

	ACubeCharacterBase* Target = Cast<ACubeCharacterBase>(Character->AttackTarget);
	if ( !IsValid(Target) ) return;
	if ( Target->ICombatInterface::Execute_IsDead(Target) ) CancelAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true);
	if ( !UCubeAbilitySystemLibrary::IsNotFriend(Character, Target) ) CancelAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true);
		
		

	ACubePlayerController* Controller = Cast<ACubePlayerController>(Character->GetController());
	if ( !IsValid(Controller) ) return;

	Distance = Character->GetDistanceTo(Character->AttackTarget);

	UAbilitySystemComponent* ASC = Cast<UAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Character));
	if ( !ASC )return;

	Range = ASC->GetNumericAttribute(UCubeAttributeSet::GetRangeAttribute());

	bAttackLoopActive = ( Distance < Range );

	if ( bAttackLoopActive )
	{
		Controller->SetPathAndRun(Character->GetActorLocation());
		//UE_LOG(LogTemp, Log, TEXT("CharacterLoc : %s"), *Character->GetActorLocation().ToString());

		if ( StoredTickEvent.IsBound() )
		{
			StoredTickEvent.Execute();
			//UE_LOG(LogTemp, Log, TEXT("블루프린트 Tick 이벤트 실행됨!"));
		}
	}
	else
	{
		Controller->SetPathAndRun(Character->AttackTarget->GetActorLocation());
		//UE_LOG(LogTemp, Log, TEXT("AttackTargetLoc : %s"), *Character->AttackTarget->GetActorLocation().ToString());
	}
}

//bool UCubeAttackAbility::PlayAttackMontage(UAnimMontage* AttackMontage, float Rate)
//{
//	if ( !bAttackLoopActive ) return false;
//
//	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
//	if ( ASC && AttackMontage )
//	{
//		ASC->PlayMontage(this, GetCurrentActivationInfo(), AttackMontage, Rate);
//	}
//	else
//	{
//		return false;
//	}
//	return true;
//}