// Fill out your copyright notice in the Description page of Project Settings.


#include "CubePlayerState.h"
#include "P_Cube/AbilitySystem/CubeAbilitySystemComponent.h"
#include "P_Cube/AbilitySystem/CubeAttributeSet.h"

ACubePlayerState::ACubePlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UCubeAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true); // 서버에서 발생한 AS의 상태 변화를 클라이언트에 반영.
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed); // 다른 플레이어들이 해당 클라이언트의 플레이어 정보에 상호작용할 수 있도록 복사.

	AttributeSet = CreateDefaultSubobject<UCubeAttributeSet>("AttributeSet");

	NetUpdateFrequency = 100.f; // 객체 상태 업데이트 주기 (초당 100)
}

UAbilitySystemComponent* ACubePlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
