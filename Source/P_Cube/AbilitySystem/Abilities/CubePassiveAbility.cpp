// Fill out your copyright notice in the Description page of Project Settings.


#include "CubePassiveAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "P_Cube/AbilitySystem/CubeAbilitySystemComponent.h"

void UCubePassiveAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, 
										  const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, 
										  const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	if ( UCubeAbilitySystemComponent* CubeASC = Cast<UCubeAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo())) )
	{
		CubeASC->DeactivatePassiveAbility.AddUObject(this, &UCubePassiveAbility::ReceiveDeactivate);
	}
}

void UCubePassiveAbility::ReceiveDeactivate(const FGameplayTag & AbilityTag)
{
	if ( AbilityTags.HasTagExact(AbilityTag) )
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}
}
