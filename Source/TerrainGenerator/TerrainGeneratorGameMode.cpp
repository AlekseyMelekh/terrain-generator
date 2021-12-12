// Copyright Epic Games, Inc. All Rights Reserved.

#include "TerrainGeneratorGameMode.h"

#include "TerrainGeneratorCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "EngineUtils.h"

ATerrainGeneratorGameMode::ATerrainGeneratorGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(
		TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

void ATerrainGeneratorGameMode::BeginPlay()
{
	Super::BeginPlay();

	// FActorSpawnParameters Params;
	// Params.Name = "Terrain Generator";
	//
	// GetWorld()->SpawnActor<AGenerator>(AGenerator::StaticClass(), {0, 0, 0}, FRotator::ZeroRotator, Params);
	//
	// for (TActorIterator<AGenerator> Iterator(GetWorld()); Iterator; ++Iterator)
	// {
	// 	if ((*Iterator)->GetName() == "Terrain Generator")
	// 	{
	// 		(*Iterator)->SetChunkSize(512);
	// 		(*Iterator)->SetDistanceBetweenVertices(100);
	// 		(*Iterator)->SetMaxHeight(10000);
	// 		(*Iterator)->Init();
	// 		break;
	// 	}
	// }
}

void ATerrainGeneratorGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}
