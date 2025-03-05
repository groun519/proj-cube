// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponWidget.h"

void UWeaponWidget::SetPlayerWeaponTag(ACubeCharacterBase* Player, FGameplayTag WeaponTag)
{
	Player->SetWeaponTag(WeaponTag);
}
