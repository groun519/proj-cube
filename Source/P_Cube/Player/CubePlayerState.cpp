// Fill out your copyright notice in the Description page of Project Settings.


#include "CubePlayerState.h"

#include "P_Cube/AbilitySystem/CubeAbilitySystemComponent.h"
#include "P_Cube/AbilitySystem/CubeAttributeSet.h"
#include "Net/UnrealNetwork.h"

ACubePlayerState::ACubePlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UCubeAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true); // �������� �߻��� AS�� ���� ��ȭ�� Ŭ���̾�Ʈ�� �ݿ�.
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed); // �ٸ� �÷��̾���� �ش� Ŭ���̾�Ʈ�� �÷��̾� ������ ��ȣ�ۿ��� �� �ֵ��� ����.

	AttributeSet = CreateDefaultSubobject<UCubeAttributeSet>("AttributeSet");

	NetUpdateFrequency = 100.f; // ��ü ���� ������Ʈ �ֱ� (�ʴ� 100)
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
