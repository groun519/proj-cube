// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "P_Cube/CubeGameplayTags.h"
#include "P_Cube/UI/WidgetController/CubeWidgetController.h"
#include "GameplayTagContainer.h"
#include "OverlayWidgetController.generated.h"

class UCubeUserWidget;
class UAbilityInfo;
class UCubeAbilitySystemComponent;

struct FCubeAbilityInfo;
USTRUCT(BlueprintType)
struct FUIWidgetRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag MessageTag = FGameplayTag();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText Message = FText();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class UCubeUserWidget> MessageWidget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UTexture2D* Image = nullptr;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeChangedSignature, float, NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMessageWidgetRowSignature, FUIWidgetRow, Row);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEnableUniqueSkillSignature, bool, bShouldAddUniqueButton);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWaitForEquipSelectionSignature, const FGameplayTag&, AbilityType);

struct FSelectedUniqueAbility
{
	FGameplayTag UniqueAbility = FGameplayTag();
	FGameplayTag Status = FGameplayTag();
};

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class P_CUBE_API UOverlayWidgetController : public UCubeWidgetController
{
	GENERATED_BODY()
	
public:
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeChangedSignature OnMaxHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeChangedSignature OnManaChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeChangedSignature OnMaxManaChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Messages")
	FMessageWidgetRowSignature MessageWidgetRowDelegate;

	UPROPERTY(BlueprintAssignable, Category = "GAS|XP")
	FOnAttributeChangedSignature OnXPPercentChangedDelegate;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Level")
	FOnPlayerStatChangedSignature OnPlayerLevelChangedDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnPlayerStatChangedSignature SkillPointsChanged;
	
	UPROPERTY(BlueprintAssignable)
	FEnableUniqueSkillSignature EnableUniqueSkillDelegate;

	UPROPERTY(BlueprintAssignable)
	FWaitForEquipSelectionSignature WaitForEquipDelegate;

	UPROPERTY(BlueprintAssignable)
	FWaitForEquipSelectionSignature StopWaitingForEquipDelegate;


	UFUNCTION(BlueprintCallable)
	void UpdateUniqueSkillEnable(const FGameplayTag& AbilityTag);

	UFUNCTION(BlueprintCallable)
	void SpendSkillPointButtonPressed(const FGameplayTag& UniqueAbilityTag, const FGameplayTag& SlotTag);

	UFUNCTION(BlueprintCallable)
	void EquipSkillBoxSelected(const FGameplayTag& AbilityTag); // 플레이어가 사용 가능한 스킬박스 클릭 시 사용될 함수.

	UFUNCTION(BlueprintCallable)
	void WeaponSkillEquipButtonPressed(const FGameplayTag& AbilityTypeTag, const FGameplayTag& AbilityTag); // 무기 오브젝트 좌클릭 시 생성되는 UI 클릭 시 사용될 함수.

	UFUNCTION(BlueprintCallable)
	void SwapSkillSlot(const FGameplayTag& AbilityTag);

	UFUNCTION(BlueprintCallable)
	void EquipSkillBoxPressed(const FGameplayTag& SlotTag, const FGameplayTag& AbilityType);
	
	UFUNCTION(BlueprintCallable)
	FGameplayTag GetUniqueTagByBasicTag(const FGameplayTag& BasicTag);

	UFUNCTION(BlueprintCallable)
	FCubeAbilityInfo GetAbilityInfoByTag(const FGameplayTag& Tag);

	void OnAbilityEquipped(const FGameplayTag& AbilityTag, const FGameplayTag& Status, const FGameplayTag& Slot, const FGameplayTag& PreviousSlot);

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widget Data")
	TObjectPtr<UDataTable> MessageWidgetDataTable;

	template<typename T>
	T* GetDataTableRowByTag(UDataTable* DataTable, const FGameplayTag& Tag);

	void OnXPChanged(int32 NewXP);

private:

	static void ShouldEnableUniqueButton(const FGameplayTag& AbilityStatus, int32 SkillPoints, bool& bShouldAddUniqueButton);
	FSelectedUniqueAbility SelectedUniqueAbility = { FCubeGameplayTags::Get().Abilities_None,  FCubeGameplayTags::Get().Abilities_Status_UnEquipped };
	int32 CurrentSkillPoints = 0;

	bool bWaitingForEquipSelection = false;

	FGameplayTag SelectedSlot;
	FGameplayTag SelectedWeaponAbility;

	FGameplayTag SelectedSwapSlot;
	FGameplayTag SelectedSwapWeaponAbility;
};

template <typename T> // MessageWidgetData 데이터테이블 값을 가지는 템플릿.
T* UOverlayWidgetController::GetDataTableRowByTag(UDataTable* DataTable, const FGameplayTag& Tag)
{
	return DataTable->FindRow<T>(Tag.GetTagName(), TEXT("")); // 행의 이름을 반환함.
}