#pragma once

#include "RuntimeMeshProvider.h"

#include "CoreMinimal.h"
#include "BiomeController.h"
#include "Chunk.generated.h"

class BiomeController;

UCLASS()
class TERRAINGENERATOR_API UChunk : public URuntimeMeshProvider
{
	GENERATED_BODY()

private:
	/* Material that we stretch on triangles
	 * TODO: Add multiple materials and stretching logic
	 */
	UPROPERTY(EditAnywhere)
	UMaterialInterface* Material;

	/*	Number of vertices in a side of one chunk
	*	The chunk itself contains ChunkSize * ChunkSize vertices
	*/
	UPROPERTY(EditAnywhere)
	uint32 ChunkSize = 1024;

	/* Distance between 2 adjacent vertices of the plain grid in world units */
	UPROPERTY(EditAnywhere)
	uint32 DistanceBetweenVertices = 25;

	/* Maximum height of a vertex in a mesh */
	UPROPERTY(EditAnywhere)
	uint32 MaxHeight = 5000;

	/* The coordinates of the start of our chunk relative to global coordinates */
	FVector2D StartCoords;
	/* This parameter is needed by RMC so that it can calculate the AABB of our mesh */
	FBoxSphereBounds LocalBounds;

	/* Need it to calculate our local Heights regarding to global */
	BiomeController* GenBiomeController;

public:
	UChunk();

	void SetChunkSize(uint32 ChunkSize_);
	void SetDistanceBetweenVertices(uint32 DistanceBetweenVertices_);
	void SetMaxHeight(uint32 MaxHeight_);
	void SetMaterial(UMaterialInterface* Material_);
	void SetBiomeController(BiomeController* BiomeController_);
	void SetStartCoord(FVector2D Coords);

protected:
	virtual void Initialize() override;
	virtual FBoxSphereBounds GetBounds() override;
	virtual bool HasCollisionMesh() override;
	virtual FRuntimeMeshCollisionSettings GetCollisionSettings() override;
	virtual bool GetCollisionMesh(FRuntimeMeshCollisionData& CollisionData) override;
	virtual bool
	GetSectionMeshForLOD(int32 LODIndex, int32 SectionId, FRuntimeMeshRenderableMeshData& MeshData) override;
	virtual bool IsThreadSafe() override;

private:
	void CalculateBounds();
};
