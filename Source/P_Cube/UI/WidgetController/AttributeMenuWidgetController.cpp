// Fill out your copyright notice in the Description page of Project Settings.


#include "AttributeMenuWidgetController.h"

#include "P_Cube/AbilitySystem/CubeAttributeSet.h"
#include "P_Cube/AbilitySystem/Data/AttributeInfo.h"

#include "P_Cube/Player/CubePlayerState.h"


void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	UCubeAttributeSet* AS = CastChecked<UCubeAttributeSet>(AttributeSet);
	check(AttributeInfo);
	for (auto& Pair : AS->TagsToAttributes)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Value()).AddLambda(
			[this, Pair](const FOnAttributeChangeData& Data)
			{
				BroadcastAttributeInfo(Pair.Key, Pair.Value());
			}
		);
	}

	ACubePlayerState* CubePlayerState = CastChecked<ACubePlayerState>(PlayerState);
	CubePlayerState->OnMoneyChangedDelegate.AddLambda(
		[this](int32 Points)
		{
			MoneyChangedDelegate.Broadcast(Points);
		}
	);
}

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	UCubeAttributeSet* AS = CastChecked<UCubeAttributeSet>(AttributeSet);
	check(AttributeInfo);

	for (auto& Pair : AS->TagsToAttributes)
	{
		BroadcastAttributeInfo(Pair.Key, Pair.Value());
	}

	ACubePlayerState* CubePlayerState = CastChecked<ACubePlayerState>(PlayerState);
	MoneyChangedDelegate.Broadcast(CubePlayerState->GetMoney());
}

void UAttributeMenuWidgetController::BroadcastAttributeInfo(const FGameplayTag& AttributeTag, const FGameplayAttribute& Attribute) const
{
	FCubeAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(AttributeTag);
	Info.AttributeValue = Attribute.GetNumericValue(AttributeSet);
	AttributeInfoDelegate.Broadcast(Info);
}
