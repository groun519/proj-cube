// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CubeCharacterBase.h"
#include "P_Cube/Interaction/EnemyInterface.h"
#include "P_Cube/Interaction/HighlightInterface.h"
#include "P_Cube/UI/WidgetController/OverlayWidgetController.h"
#include "CubeEnemy.generated.h"

class UWidgetComponent;
class UBehaviorTree;
class ACubeAIController;

/**
 * 
 */
UCLASS()
class P_CUBE_API ACubeEnemy : public ACubeCharacterBase, public IEnemyInterface, public	IHighlightInterface
{
	GENERATED_BODY()

public:
	ACubeEnemy();
	virtual void PossessedBy(AController* NewController) override;

	/** Highlight Interface */
	virtual void HighlightActor_Implementation() override;
	virtual void UnHighlightActor_Implementation() override;
	virtual void SetMoveToLocation_Implementation(FVector& OutDestination) override;
	/** end Highlight Interface */

	/** Combat Interface */
	virtual int32 GetPlayerLevel_Implementation() override;
	virtual void Die() override;
	virtual void SetCombatTarget_Implementation(AActor* InCombatTarget) override;
	virtual AActor* GetCombatTarget_Implementation() const override;
	/** end Combat Interface */

	UPROPERTY(BlueprintReadWrite, Category = "Combat")
	TObjectPtr<AActor> CombatTarget;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnMaxHealthChanged;

	void HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount);

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	bool bHitReacting = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	float LifeSpan = 5.f;

protected:
	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo() override;
	virtual void InitializeDefaultAttributes() const override;
	virtual void StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Defaults")
	int32 Level = 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> HealthBar;

	UPROPERTY(EditAnywhere, Category = "AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;

	UPROPERTY()
	TObjectPtr<ACubeAIController> CubeAIController;

	UFUNCTION(BlueprintImplementableEvent)
	void SpawnLoot();
};
