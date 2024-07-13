// Fill out your copyright notice in the Description page of Project Settings.


#include "P_Cube/UI/WidgetController/OverlayWidgetController.h"

#include "P_Cube/AbilitySystem/CubeAttributeSet.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	const UCubeAttributeSet* CubeAttributeSet = CastChecked<UCubeAttributeSet>(AttributeSet);

	OnHealthChanged.Broadcast(CubeAttributeSet->GetHealth()); // ü�� ���� ����Ǹ� ��ε�ĳ���� �Ѵ�. (OnHealthChanged_Event �������Ʈ�� Ʈ���� ��.)
	OnMaxHealthChanged.Broadcast(CubeAttributeSet->GetMaxHealth()); // ���������� �ִ�ü��
	OnManaChanged.Broadcast(CubeAttributeSet->GetMana()); // ����
	OnMaxManaChanged.Broadcast(CubeAttributeSet->GetMaxMana()); // �ִ븶��
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
	OnHealthChanged.Broadcast(Data.NewValue); // ����� ü�� ���� ��ε�ĳ���� �Ѵ�.
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
