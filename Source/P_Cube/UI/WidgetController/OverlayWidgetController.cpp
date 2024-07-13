// Fill out your copyright notice in the Description page of Project Settings.


#include "P_Cube/UI/WidgetController/OverlayWidgetController.h"

#include "P_Cube/AbilitySystem/CubeAttributeSet.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	const UCubeAttributeSet* CubeAttributeSet = CastChecked<UCubeAttributeSet>(AttributeSet);

	OnHealthChanged.Broadcast(CubeAttributeSet->GetHealth()); // 체력 값이 변경되면 브로드캐스팅 한다. (OnHealthChanged_Event 블루프린트가 트리거 됨.)
	OnMaxHealthChanged.Broadcast(CubeAttributeSet->GetMaxHealth()); // 마찬가지로 최대체력
	OnManaChanged.Broadcast(CubeAttributeSet->GetMana()); // 마나
	OnMaxManaChanged.Broadcast(CubeAttributeSet->GetMaxMana()); // 최대마나
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	const UCubeAttributeSet* CubeAttributeSet = CastChecked<UCubeAttributeSet>(AttributeSet);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate( //?
		CubeAttributeSet->GetHealthAttribute()).AddUObject(this, &UOverlayWidgetController::HealthChanged);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		CubeAttributeSet->GetMaxHealthAttribute()).AddUObject(this, &UOverlayWidgetController::MaxHealthChanged);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		CubeAttributeSet->GetManaAttribute()).AddUObject(this, &UOverlayWidgetController::ManaChanged);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		CubeAttributeSet->GetMaxManaAttribute()).AddUObject(this, &UOverlayWidgetController::MaxManaChanged);
}

void UOverlayWidgetController::HealthChanged(const FOnAttributeChangeData& Data) const
{
	OnHealthChanged.Broadcast(Data.NewValue); // 변경된 체력 값을 브로드캐스팅 한다.
}

void UOverlayWidgetController::MaxHealthChanged(const FOnAttributeChangeData& Data) const
{
	OnMaxHealthChanged.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::ManaChanged(const FOnAttributeChangeData& Data) const
{
	OnManaChanged.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::MaxManaChanged(const FOnAttributeChangeData& Data) const
{
	OnMaxManaChanged.Broadcast(Data.NewValue);
}
