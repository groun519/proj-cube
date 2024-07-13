// Fill out your copyright notice in the Description page of Project Settings.


#include "CubeAttributeSet.h"
#include "Net/UnrealNetwork.h"

UCubeAttributeSet::UCubeAttributeSet()
{
	InitHealth(10.f);
	InitMaxHealth(100.f);
	InitMana(50.f);
	InitMaxMana(50.f);
}

void UCubeAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const // �������� ��������� �߻��� ���� �� �Ӽ�(attribute)���� ������ �������� �Լ�.
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UCubeAttributeSet, Health, COND_None, REPNOTIFY_Always); // �Ӽ��� ��������� ����� �Ӽ��� ����(�׻�)�ϰ�, OnRep_�Լ��� ȣ����.
	DOREPLIFETIME_CONDITION_NOTIFY(UCubeAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
}

void UCubeAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const 
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCubeAttributeSet, Health, OldHealth); // �������� �Ӽ��� ����Ǿ��ٸ�, Ŭ���̾�Ʈ���� �ش� �Ӽ��� ��������� ����.
}

void UCubeAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCubeAttributeSet, MaxHealth, OldMaxHealth);
}

void UCubeAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCubeAttributeSet, Mana, OldMana);
}

void UCubeAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCubeAttributeSet, MaxMana, OldMaxMana);
}
