// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TerrainGeneratorGameMode.generated.h"

UCLASS(minimalapi)
class ATerrainGeneratorGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ATerrainGeneratorGameMode();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
};



