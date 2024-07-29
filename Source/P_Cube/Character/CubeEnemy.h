// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CubeCharacterBase.h"
#include "P_Cube/Interaction/EnemyInterface.h"
#include "CubeEnemy.generated.h"

/**
 * 
 */
UCLASS()
class P_CUBE_API ACubeEnemy : public ACubeCharacterBase, public IEnemyInterface
{
	GENERATED_BODY()

public:
	ACubeEnemy();

	/** Enemy Interface */
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;
	/** end Enemy Interface */

	/** Combat Interface */
	virtual int32 GetPlayerLevel() override;
	/** end Combat Interface */

protected:
	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Defaults")
	int32 Level = 1;
};
