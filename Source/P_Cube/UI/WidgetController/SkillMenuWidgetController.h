// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "P_Cube/CubeGameplayTags.h"
#include "P_Cube/UI/WidgetController/CubeWidgetController.h"
#include "GameplayTagContainer.h"
#include "SkillMenuWidgetController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FSkillMenuBoxSelectedSignature, FString, BasicDescriptionString, FString, UniqueDescriptionString);
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSkillBoxReassignedSignature, const FGameplayTag&, AbilityTag);

struct FSelectedAbility
{
	FGameplayTag Ability = FGameplayTag();
	FGameplayTag Status = FGameplayTag();
};

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class P_CUBE_API USkillMenuWidgetController : public UCubeWidgetController
{
	GENERATED_BODY()
public:
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;

	UPROPERTY(BlueprintAssignable)
	FOnPlayerStatChangedSignature SkillPointsChanged;

	UPROPERTY(BlueprintAssignable)
	FSkillMenuBoxSelectedSignature SkillMenuBoxSelectedDelegate;

	/*UPROPERTY(BlueprintAssignable)
	FSkillBoxReassignedSignature SkillBoxReassignedDelegate;*/

	UFUNCTION(BlueprintCallable)
	void SkillBoxSelected(const FGameplayTag& AbilityTag);

	UFUNCTION(BlueprintCallable)
	void BoxDeselect();

protected:
	//void OnAbilityEquipped(const FGameplayTag& AbilityTag, const FGameplayTag& Status, const FGameplayTag& Slot, const FGameplayTag& PreviousSlot);

private:
	FSelectedAbility SelectedAbility = { FCubeGameplayTags::Get().Abilities_None,  FCubeGameplayTags::Get().Abilities_Status_UnEquipped };
	int32 CurrentSkillPoints = 0;
};
