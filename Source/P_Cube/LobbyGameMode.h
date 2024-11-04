// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "P_Cube/AbilitySystem/Data/CharacterClassInfo.h"
#include "LobbyGameMode.generated.h"

/**
 * 
 */
UCLASS()
class P_CUBE_API ALobbyGameMode : public AGameMode
{
	GENERATED_BODY()
public:

	ALobbyGameMode();

	virtual void PostLogin(APlayerController* NewPlayer) override;
	/*void BeginPlay();*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxTravelPlayers = 4;

private:
	UFUNCTION(BlueprintCallable)
	void GoTravel(int32 ReadyPlayers);

	/*TArray<ECharacterClass> ClassArray;*/
};
