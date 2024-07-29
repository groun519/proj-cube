// Fill out your copyright notice in the Description page of Project Settings.


#include "P_Cube/interaction/CombatInterface.h"

// Add default functionality here for any ICombatInterface functions that are not pure virtual.

int32 ICombatInterface::GetPlayerLevel()
{
	return 0;
}

FVector ICombatInterface::GetCombatSocketLocation()
{
	return FVector();
}

// TODO : 나중에 여기다 플레이어 위치 getter 만들까 싶음.