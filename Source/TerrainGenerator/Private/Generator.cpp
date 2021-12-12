#include "Generator.h"

#include "Chunk.h"

void AGenerator::SetE1(float E1)
{
	GenBiomeController->E1 = E1;
	RemoveAllChunks();
}

void AGenerator::SetE2(float E2)
{
	GenBiomeController->E2 = E2;
	RemoveAllChunks();
}

void AGenerator::SetE3(float E3)
{
	GenBiomeController->E3 = E3;
	RemoveAllChunks();
}

void AGenerator::SetE4(float E4)
{
	GenBiomeController->E4 = E4;
	RemoveAllChunks();
}

void AGenerator::SetE5(float E5)
{
	GenBiomeController->E5 = E5;
	RemoveAllChunks();
}

void AGenerator::SetE6(float E6)
{
	GenBiomeController->E6 = E6;
	RemoveAllChunks();
}

void AGenerator::SetExp(float Exp)
{
	GenBiomeController->Exp = Exp * 15.0;
	RemoveAllChunks();
}

float AGenerator::GetE1() const
{
	return GenBiomeController->E1;
}

float AGenerator::GetE2() const
{
	return GenBiomeController->E2;
}

float AGenerator::GetE3() const
{
	return GenBiomeController->E3;
}

float AGenerator::GetE4() const
{
	return GenBiomeController->E4;
}

float AGenerator::GetE5() const
{
	return GenBiomeController->E5;
}

float AGenerator::GetE6() const
{
	return GenBiomeController->E6;
}

float AGenerator::GetExp() const
{
	return GenBiomeController->Exp / 15.0;
}

AGenerator::AGenerator()
{
	GenBiomeController = new BiomeController(this);
	TickedTime = 0;
	PrimaryActorTick.bCanEverTick = true;
}

void AGenerator::SetChunkSize(uint32 ChunkSize_)
{
	ChunkSize = ChunkSize_;
}

void AGenerator::SetDistanceBetweenVertices(uint32 DistanceBetweenVertices_)
{
	DistanceBetweenVertices = DistanceBetweenVertices_;
}

void AGenerator::SetMaxHeight(uint32 MaxHeight_)
{
	MaxHeight = MaxHeight_;
}

bool AGenerator::Init()
{
	// TODO: Remove this
	constexpr int Infinity = 1'000'000; 
	GenBiomeController->SetWorldSize(Infinity);
	if (!GenBiomeController->Work())
	{
		UE_LOG(LogTemp, Fatal, TEXT("The world generation fucked up!"));
	}

	return true;
}

URuntimeMeshComponent* AGenerator::RegisterNewChunk(FVector2D Coords)
{
	UChunk* ChunkProvider = NewObject<UChunk>(this);
	if (ChunkProvider)
	{
		ChunkProvider->SetChunkSize(ChunkSize);
		ChunkProvider->SetDistanceBetweenVertices(DistanceBetweenVertices);
		ChunkProvider->SetMaxHeight(MaxHeight);
		ChunkProvider->SetMaterial(Material);
		ChunkProvider->SetBiomeController(GenBiomeController);
		ChunkProvider->SetStartCoord(Coords);

		URuntimeMeshComponent* Chunk = NewObject<URuntimeMeshComponent>(this);
		Chunk->RegisterComponent();
		const FAttachmentTransformRules Rules = FAttachmentTransformRules(EAttachmentRule::KeepWorld,
		                                                                  EAttachmentRule::KeepWorld,
		                                                                  EAttachmentRule::KeepWorld, false);
		Chunk->AttachToComponent(GetRuntimeMeshComponent(), Rules);
		Chunk->Initialize(ChunkProvider);

		return Chunk;
	}

	return nullptr;
}

void AGenerator::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
}

void AGenerator::BeginPlay()
{
	Super::BeginPlay();
	Init();
}

void AGenerator::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	TickedTime += DeltaSeconds;

	FVector PlayerLocation = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
	ReloadChunks({PlayerLocation.X, PlayerLocation.Y});
}

void AGenerator::ReloadChunks(const FVector2D& PlayerCoords)
{
	RemoveFarChunks(PlayerCoords);
	SpawnChunks(PlayerCoords);
}

bool AGenerator::ChunkIsFarFromPlayer(const FVector2D& PlayerCoords, const FVector2D& ChunkCoords) const
{
	const int32 ChunkCenterX = ChunkCoords.X + ((ChunkSize * DistanceBetweenVertices) >> 1);
	const int32 ChunkCenterY = ChunkCoords.Y + ((ChunkSize * DistanceBetweenVertices) >> 1);
	const float XDiff = PlayerCoords.X - ChunkCenterX;
	const float YDiff = PlayerCoords.Y - ChunkCenterY;

	return FMath::Abs(XDiff) > RenderDistance || FMath::Abs(YDiff) > RenderDistance;
}

void AGenerator::RemoveFarChunks(const FVector2D& PlayerCoords)
{
	TSet<FVector2D> ChunksToRemove;
	for (auto& ChunkData : RenderedChunks)
	{
		if (ChunkIsFarFromPlayer(PlayerCoords, ChunkData.Key))
		{
			ChunksToRemove.Add(ChunkData.Key);
			ChunkData.Value->DestroyComponent(true);
		}
	}
	for (const auto& Chunk : ChunksToRemove)
	{
		RenderedChunks.Remove(Chunk);
	}
}

void AGenerator::SpawnChunks(const FVector2D& PlayerCoords)
{
	const float Size = (ChunkSize - 1) * DistanceBetweenVertices;
	/* We take the random nearest chunk boundary to count the rest */
	const FVector2D RandomBorder(static_cast<int>(PlayerCoords.X / Size) * Size,
	                             static_cast<int>(PlayerCoords.Y / Size) * Size);

	for (int i = -(RenderDistance / Size) - 3; i < (RenderDistance / Size) + 3; ++i)
	{
		for (int j = -(RenderDistance / Size) - 3; j < (RenderDistance / Size) + 3; ++j)
		{
			FVector2D ChunkBorder(RandomBorder.X + i * Size, RandomBorder.Y + j * Size);
			if (!ChunkIsFarFromPlayer(PlayerCoords, ChunkBorder))
			{
				if (!RenderedChunks.Contains(ChunkBorder))
				{
					auto Chunk = RegisterNewChunk(ChunkBorder);
					RenderedChunks.Emplace(ChunkBorder, Chunk);
				}
			}
		}
	}
}

void AGenerator::RemoveAllChunks()
{
	for (auto& ChunkData : RenderedChunks)
	{
		ChunkData.Value->DestroyComponent(true);
	}
	RenderedChunks.Empty();
}
