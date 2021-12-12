#include "BiomeController.h"

BiomeController::BiomeController(AGenerator* Owner_) : Size(0), Owner(Owner_)
{
	Generator.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
	Generator.SetSeed(Random.GetRandom());
	Generator.SetFractalType(FastNoiseLite::FractalType_PingPong);
}

void BiomeController::SetWorldSize(int Size_)
{
	Size = Size_;
}

uint32 BiomeController::GetHeightAtPoint(float X, float Y)
{
	if (!Ready)
	{
		UE_LOG(LogTemp, Fatal, TEXT("Controller not ready yet!"));

		return 0;
	}
	
	// TODO: Refactor this
	const uint32 x = X / Owner->GetDistanceBetweenVertices() + Size;
	const uint32 y = Y / Owner->GetDistanceBetweenVertices() + Size;
	const uint32 z = GetNoise(x, y) * Owner->GetMaxHeight();

	return z;
}

bool BiomeController::Work()
{
	for (int i = 0; i < 1; ++i)
	{
		if (!GenerateWorldHeightMap())
		{
			UE_LOG(LogTemp, Warning, TEXT("The height map was generated incorrectly!"));

			return false;
		}
	}

	return Ready = true;
}

bool BiomeController::GenerateWorldHeightMap()
{
	return true;
}

float BiomeController::GetNoise(float X, float Y)
{
	auto Noise = [&](const float nx, const float ny)
	{
		return Generator.GetNoise(nx, ny) / 2 + 0.5;
	};

	//TODO: Refactor this
	float nx = X / (Size * 2.f) - 0.5, ny = Y / (Size * 2.f) - 0.5;
	nx *= 300000;
	ny *= 300000;
	float e = (E1 * Noise(1 * nx, 1 * ny)
		+ E2 * Noise(2 * nx, 2 * ny)
		+ E3 * Noise(4 * nx, 4 * ny)
		+ E4 * Noise(8 * nx, 8 * ny)
		+ E5 * Noise(16 * nx, 16 * ny)
		+ E6 * Noise(32 * nx, 32 * ny)
	);
	e /= (E1 + E2 + E3 + E4 + E5 + E6);

	return FMath::Pow(e, Exp);
}
