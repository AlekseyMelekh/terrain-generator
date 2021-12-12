#pragma once

#include "Generator.h"
#include "TerrainGenerator/Utils/FastNoiseLite.h"
#include "TerrainGenerator/Utils/Randomer.h"

class AGenerator;

class BiomeController
{
private:
	uint32 Size;
	bool Ready = false;

	AGenerator* Owner;

	Randomer Random;
	FastNoiseLite Generator;

public:
	/* Noise constants. */
	float E1 = 1.0, E2 = 0.5, E3 = 0.25;
	float E4 = 0.13, E5 = 0.06, E6 = 0.03, Exp = 5.29;
	/* ---------------- */
	
public:
	BiomeController(AGenerator* Owner_);

	void SetWorldSize(int Size);
	uint32 GetHeightAtPoint(float X, float Y);
	// Prepares the world for generation
	bool Work();
	
private:
	bool GenerateWorldHeightMap();
	float GetNoise(float X, float Y);
};
