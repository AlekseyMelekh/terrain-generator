#pragma once

#include "BiomeController.h"
#include "RuntimeMeshActor.h"
#include "Delegates/Delegate.h"
#include "CoreMinimal.h"

#include "Generator.generated.h"

class BiomeController;
class UChunk;

DECLARE_MULTICAST_DELEGATE(FGenerator)

UCLASS()
class TERRAINGENERATOR_API AGenerator : public ARuntimeMeshActor
{
	GENERATED_BODY()

	/* Interface for configuring chunks */
	UPROPERTY(EditAnywhere)
	UMaterialInterface* Material;

	UPROPERTY(EditAnywhere)
	uint32 ChunkSize = 64;

	UPROPERTY(EditAnywhere)
	uint32 DistanceBetweenVertices = 100;

	UPROPERTY(EditAnywhere)
	uint32 MaxHeight = 5000;
	/* -------------------------------- */

	/* Interface for configuring Noise */
	UFUNCTION(BlueprintCallable)
	void SetE1(float E1);
	
	UFUNCTION(BlueprintCallable, Category="Setters")
	void SetE2(float E2);
	
	UFUNCTION(BlueprintCallable, Category="Setters")
	void SetE3(float E3);
	
	UFUNCTION(BlueprintCallable, Category="Setters")
	void SetE4(float E4);
	
	UFUNCTION(BlueprintCallable, Category="Setters")
	void SetE5(float E5);
	
	UFUNCTION(BlueprintCallable, Category="Setters")
	void SetE6(float E6);
	
	UFUNCTION(BlueprintCallable, Category="Setters")
	void SetExp(float Exp);

	UFUNCTION(BlueprintCallable, Category = "Getters")
	float GetE1() const;
	
	UFUNCTION(BlueprintCallable, Category = "Getters")
	float GetE2() const;
	
	UFUNCTION(BlueprintCallable, Category = "Getters")
	float GetE3() const;
	
	UFUNCTION(BlueprintCallable, Category = "Getters")
	float GetE4() const;
	
	UFUNCTION(BlueprintCallable, Category = "Getters")
	float GetE5() const;
	
	UFUNCTION(BlueprintCallable, Category = "Getters")
	float GetE6() const;
	
	UFUNCTION(BlueprintCallable, Category = "Getters")
	float GetExp() const;
	/* -------------------------------- */

	float TickedTime;

	BiomeController* GenBiomeController;
	TMap<FVector2D, URuntimeMeshComponent*> RenderedChunks;

	UPROPERTY(EditAnywhere)
	uint32 RenderDistance = 1000;

public:
	AGenerator();

	void SetChunkSize(uint32 ChunkSize_);
	void SetDistanceBetweenVertices(uint32 DistanceBetweenVertices_);
	void SetMaxHeight(uint32 MaxHeight_);

	uint32 GetChunkSize() const { return ChunkSize; }
	uint32 GetDistanceBetweenVertices() const { return DistanceBetweenVertices; }
	uint32 GetMaxHeight() const { return MaxHeight; }

	bool Init();
	URuntimeMeshComponent* RegisterNewChunk(FVector2D Coords);

	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	// TODO: Remove flicker effect
	void ReloadChunks(const FVector2D& PlayerCoords);
	bool ChunkIsFarFromPlayer(const FVector2D& PlayerCoords, const FVector2D& ChunkCoords) const;
	void RemoveFarChunks(const FVector2D& PlayerCoords);
	void SpawnChunks(const FVector2D& PlayerCoords);
	void RemoveAllChunks();
};
