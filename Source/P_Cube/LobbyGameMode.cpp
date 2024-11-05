// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"
#include "GameFramework/GameStateBase.h"
#include "P_Cube/Character/CubeCharacterBase.h"

ALobbyGameMode::ALobbyGameMode()
{
	/*ClassArray.Add(ECharacterClass::Warrior);
	ClassArray.Add(ECharacterClass::Ranger);
	ClassArray.Add(ECharacterClass::Wizard);
	ClassArray.Add(ECharacterClass::Healer);*/
}

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	int32 NumberOfPlayers = GameState.Get()->PlayerArray.Num();
	if (NumberOfPlayers == MaxTravelPlayers)
	{
		UWorld* World = GetWorld();
		if (World)
		{
			bUseSeamlessTravel = true;
			World->ServerTravel(FString("/Game/Maps/TestMap?listen"));
		}
	}
}

//void ALobbyGameMode::BeginPlay()
//{
//	Super::BeginPlay();
//
//	int32 NumberOfPlayers = GameState.Get()->PlayerArray.Num();
//	ECharacterClass AssignedClass = ClassArray[ NumberOfPlayers - 1 ];
//
//	if ( APawn* PlayerPawn = NewPlayer->GetPawn() )
//	{
//		if ( ACubeCharacterBase* CubeCharacter = Cast<ACubeCharacterBase>(PlayerPawn) )
//		{
//			CubeCharacter->CharacterClass = AssignedClass;
//		}
//	}
//
//	if ( ALobbyGameMode* LobbyGameMode = Cast<ALobbyGameMode>(GetWorld()->GetAuthGameMode()) )
//	{
//		int32 NumberOfPlayers = GetWorld()->GetGameState()->PlayerArray.Num();
//		if ( NumberOfPlayers <= LobbyGameMode->ClassArray.Num() )
//		{
//			ECharacterClass AssignedClass = LobbyGameMode->ClassArray[ NumberOfPlayers - 1 ];
//			CharacterClass = AssignedClass;
//		}
//	}
//}

void ALobbyGameMode::GoTravel(const int32 ReadyPlayers)
{
	int32 NumberOfPlayers = GameState.Get()->PlayerArray.Num();
	if ( ReadyPlayers == MaxTravelPlayers && NumberOfPlayers == MaxTravelPlayers )
	{
		UWorld* World = GetWorld();
		if ( World )
		{
			bUseSeamlessTravel = true;
			World->ServerTravel(FString("/Game/Maps/TestMap?listen"));
		}
	}
}
