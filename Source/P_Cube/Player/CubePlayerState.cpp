// Fill out your copyright notice in the Description page of Project Settings.


#include "CubePlayerState.h"

#include "P_Cube/AbilitySystem/CubeAbilitySystemComponent.h"
#include "P_Cube/AbilitySystem/CubeAttributeSet.h"
#include "Net/UnrealNetwork.h"

ACubePlayerState::ACubePlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UCubeAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true); // 서버에서 발생한 AS의 상태 변화를 클라이언트에 반영.
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed); // 다른 플레이어들이 해당 클라이언트의 플레이어 정보에 상호작용할 수 있도록 복사.

	AttributeSet = CreateDefaultSubobject<UCubeAttributeSet>("AttributeSet");

	NetUpdateFrequency = 100.f; // 객체 상태 업데이트 주기 (초당 100)
}

void ACubePlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACubePlayerState, Level);
	DOREPLIFETIME(ACubePlayerState, XP);
	DOREPLIFETIME(ACubePlayerState, Money);
	DOREPLIFETIME(ACubePlayerState, SkillPoints);
}

UAbilitySystemComponent* ACubePlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ACubePlayerState::AddToXP(int32 InXP)
{
	XP += InXP;
	OnXPChangedDelegate.Broadcast(XP);
}

void ACubePlayerState::AddToLevel(int32 InLevel)
{
	Level += InLevel;
	OnLevelChangedDelegate.Broadcast(Level);
}

void ACubePlayerState::AddToMoney(int32 InMoney)
{
	Money += InMoney;
	OnMoneyChangedDelegate.Broadcast(Money);
}

void ACubePlayerState::AddToSkillPoints(int32 InPoints)
{
	SkillPoints += InPoints;
	OnSkillPointsChangedDelegate.Broadcast(SkillPoints);
}

void ACubePlayerState::SetXP(int32 InXP)
{
	XP = InXP;
	OnXPChangedDelegate.Broadcast(XP);
}

void ACubePlayerState::SetLevel(int32 InLevel)
{
	Level = InLevel;
	OnLevelChangedDelegate.Broadcast(Level);
}

void ACubePlayerState::OnRep_Level(int32 OldLevel)
{
	OnLevelChangedDelegate.Broadcast(Level);
}

void ACubePlayerState::OnRep_XP(int32 OldXP)
{
	OnXPChangedDelegate.Broadcast(XP);
}

void ACubePlayerState::OnRep_Money(int32 OldMoney)
{
	OnMoneyChangedDelegate.Broadcast(Money);
}

void ACubePlayerState::OnRep_SkillPoints(int32 OldSkillPoints)
{
	OnSkillPointsChangedDelegate.Broadcast(SkillPoints);
}
