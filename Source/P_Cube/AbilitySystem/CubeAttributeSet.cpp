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

void UCubeAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const // 서버에서 변경사항이 발생해 복제 될 속성(attribute)들의 정보를 가져오는 함수.
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UCubeAttributeSet, Health, COND_None, REPNOTIFY_Always); // 속성의 변경사항이 생기면 속성을 복제(항상)하고, OnRep_함수를 호출함.
	DOREPLIFETIME_CONDITION_NOTIFY(UCubeAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
}

void UCubeAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const 
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCubeAttributeSet, Health, OldHealth); // 서버에서 속성이 변경되었다면, 클라이언트에서 해당 속성의 변경사항을 적용.
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
