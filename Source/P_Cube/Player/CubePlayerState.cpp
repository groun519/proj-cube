// Fill out your copyright notice in the Description page of Project Settings.


#include "CubePlayerState.h"
#include "P_Cube/AbilitySystem/CubeAbilitySystemComponent.h"
#include "P_Cube/AbilitySystem/CubeAttributeSet.h"

ACubePlayerState::ACubePlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UCubeAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true); // �������� �߻��� AS�� ���� ��ȭ�� Ŭ���̾�Ʈ�� �ݿ�.
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed); // �ٸ� �÷��̾���� �ش� Ŭ���̾�Ʈ�� �÷��̾� ������ ��ȣ�ۿ��� �� �ֵ��� ����.

	AttributeSet = CreateDefaultSubobject<UCubeAttributeSet>("AttributeSet");

	NetUpdateFrequency = 100.f; // ��ü ���� ������Ʈ �ֱ� (�ʴ� 100)
}

UAbilitySystemComponent* ACubePlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
