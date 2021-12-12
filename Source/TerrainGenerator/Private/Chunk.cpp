#include "Chunk.h"

UChunk::UChunk()
{
	StartCoords = FVector2D(0, 0);
	CalculateBounds();
}

void UChunk::SetChunkSize(uint32 ChunkSize_)
{
	ChunkSize = ChunkSize_;

	/* The dimensions of the chunk have changed => AABB has changed */
	CalculateBounds();
}

void UChunk::SetDistanceBetweenVertices(uint32 DistanceBetweenVertices_)
{
	DistanceBetweenVertices = DistanceBetweenVertices_;

	/* The dimensions of the chunk have changed => AABB has changed */
	CalculateBounds();
}

void UChunk::SetMaxHeight(uint32 MaxHeight_)
{
	MaxHeight = MaxHeight_;

	/* The dimensions of the chunk have changed => AABB has changed */
	CalculateBounds();
}

void UChunk::SetMaterial(UMaterialInterface* Material_)
{
	Material = Material_;
}

void UChunk::SetBiomeController(BiomeController* BiomeController_)
{
	GenBiomeController = BiomeController_;
}

void UChunk::SetStartCoord(FVector2D Coords)
{
	StartCoords = Coords;

	/* The dimensions of the chunk have changed => AABB has changed */
	CalculateBounds();
}

void UChunk::Initialize()
{
	/* Configuring RMC for further work */
	SetupMaterialSlot(0, FName("Material"), Material);
	TArray<FRuntimeMeshLODProperties> LODs;
	FRuntimeMeshLODProperties LODProperties;
	LODProperties.ScreenSize = 0.0f;
	LODs.Add(LODProperties);
	ConfigureLODs(LODs);

	FRuntimeMeshSectionProperties Properties;
	Properties.bCastsShadow = true;
	Properties.bIsVisible = true;
	Properties.MaterialSlot = 0;
	Properties.bWants32BitIndices = true;
	Properties.UpdateFrequency = ERuntimeMeshUpdateFrequency::Frequent;
	CreateSection(0, 0, Properties);

	MarkCollisionDirty();
	MarkAllLODsDirty();
}

FBoxSphereBounds UChunk::GetBounds()
{
	return LocalBounds;
}

bool UChunk::HasCollisionMesh()
{
	return true;
}

FRuntimeMeshCollisionSettings UChunk::GetCollisionSettings()
{
	FRuntimeMeshCollisionSettings Settings;
	Settings.bUseAsyncCooking = true;
	Settings.bUseComplexAsSimple = true;

	return Settings;
}

bool UChunk::GetCollisionMesh(FRuntimeMeshCollisionData& CollisionData)
{
	// We do the same as with a mesh to add a collision mesh
	for (uint32 i = 0; i < ChunkSize; ++i)
	{
		for (uint32 j = 0; j < ChunkSize; ++j)
		{
			float X = StartCoords.X + DistanceBetweenVertices * i;
			float Y = StartCoords.Y + DistanceBetweenVertices * j;
			CollisionData.Vertices.Add(FVector(X, Y, GenBiomeController->GetHeightAtPoint(X, Y)));
		}
	}

	for (uint32 i = 0; i + 1 < ChunkSize; ++i)
	{
		for (uint32 j = 0; j + 1 < ChunkSize; ++j)
		{
			CollisionData.Triangles.Add(i * ChunkSize + j, i * ChunkSize + j + 1, (i + 1) * ChunkSize + j + 1);
			CollisionData.Triangles.Add(i * ChunkSize + j, (i + 1) * ChunkSize + j + 1, (i + 1) * ChunkSize + j);
		}
	}

	return true;
}

bool UChunk::GetSectionMeshForLOD(int32 LODIndex, int32 SectionId, FRuntimeMeshRenderableMeshData& MeshData)
{
	check(SectionId == 0 && LODIndex == 0);

	/* auto CalcNormal = [](const FVector& p1, const FVector& p2, const FVector& p3) -> FVector
	{
		FVector a = p2 - p1;
		FVector b = p3 - p1;
		FVector c = a ^ b;
		c.Normalize();
		return c;
	}; */

	// Generating a square grid of (ChunkSize * ChunkSize) vertices
	// The vertices will have height determined by some noise function
	// Each 4 adjasent vertices will be connected into a cell (2 polygons)

	// Add vertices and texture coordinates to MeshData
	for (uint32 i = 0; i < ChunkSize; ++i)
	{
		for (uint32 j = 0; j < ChunkSize; ++j)
		{
			float X = StartCoords.X + DistanceBetweenVertices * i;
			float Y = StartCoords.Y + DistanceBetweenVertices * j;
			MeshData.Positions.Add(FVector(X, Y, GenBiomeController->GetHeightAtPoint(X, Y)));
			MeshData.Colors.Add(FColor::Red);

			// Temp solution, otherwise mesh would not be spawn
			// TODO: Delete this
			FVector Normal(1, 1, 1);
			Normal.Normalize();
			FVector Tangent(1, 1, 1);
			Tangent.Normalize();

			FVector2D UV(X, Y);

			MeshData.Tangents.Add(Normal, Tangent);
			MeshData.TexCoords.Add(UV);
		}
	}

	// Add triangles to MeshData
	// TODO: Add powerful normal counting
	for (uint32 i = 0; i + 1 < ChunkSize; ++i)
	{
		for (uint32 j = 0; j + 1 < ChunkSize; ++j)
		{
			MeshData.Triangles.AddTriangle(i * ChunkSize + j, i * ChunkSize + j + 1, (i + 1) * ChunkSize + j + 1);
			// FVector Normal1 = CalcNormal(MeshData.Positions.GetPosition(i * ChunkSize + j),
			// 							MeshData.Positions.GetPosition(i * ChunkSize + j + 1),
			// 							MeshData.Positions.GetPosition((i + 1) * ChunkSize + j + 1));
			MeshData.Triangles.AddTriangle(i * ChunkSize + j, (i + 1) * ChunkSize + j + 1, (i + 1) * ChunkSize + j);
			// FVector Normal2 = CalcNormal(MeshData.Positions.GetPosition(i * ChunkSize + j),
			// 							MeshData.Positions.GetPosition((i + 1) * ChunkSize + j + 1),
			// 							MeshData.Positions.GetPosition((i + 1) * ChunkSize + j));
			//
			// FVector Tangent(1, 1, 1);
			// Tangent.Normalize();
			//
			// MeshData.Tangents.Add(Normal1, Tangent);
			// MeshData.Tangents.Add(Normal1, Tangent);
			// MeshData.Tangents.Add(Normal1, Tangent);
			// MeshData.Tangents.Add(Normal2, Tangent);
			// MeshData.Tangents.Add(Normal2, Tangent);
			// MeshData.Tangents.Add(Normal2, Tangent);
		}
	}

	return true;
}

bool UChunk::IsThreadSafe()
{
	return true;
}

void UChunk::CalculateBounds()
{
	const int32 MaxCoordinateValue = ChunkSize * DistanceBetweenVertices;
	const FBox BoxBounds(FVector(StartCoords.X, StartCoords.Y, 0),
	                     FVector(StartCoords.X + MaxCoordinateValue, StartCoords.Y + MaxCoordinateValue,
	                             MaxHeight << 1));
	LocalBounds = FBoxSphereBounds(BoxBounds);
}
