

#pragma once
#include "FLevelCell.h"
#include "GridRow.h"
#include "FSize.h"
#include "GameFramework/Actor.h"
#include "LevelGenerator.generated.h"

/**
 * 
 */
UCLASS()
class ALevelGenerator : public AActor
{
	GENERATED_UCLASS_BODY()

	TArray<UGridRow> grid;
	//UPROPERTY(VisibleAnywhere, Blueprintreadonly,Category = LevelGeneration)
	FSize GridSize;

	FSize TileSize;
	TSubobjectPtr<USphereComponent> DrawSphereComponent;
	UStaticMesh* wallMesh;
	

	UFUNCTION(BlueprintCallable, Category = LevelGeneration)
	virtual void GenerateLevelWithSeed(int32 seed);

	UFUNCTION(BlueprintCallable, Category = LevelGeneration)
	bool IsValidGridCoordinate(FVector2D Coordinate);

	virtual FLevelCell* LevelCellForGridCoordinate(FVector2D Coordinate);
	
	UFUNCTION(BlueprintCallable, Category = LevelGeneration)
	FVector VectorForGridCoordinate(FVector2D Coordinate);

	UFUNCTION(BluePrintCallable, Category = LevelGeneration)
	virtual void InitializeArray(UWorld *world);

	UFUNCTION(BluePrintCallable, Category = LevelGeneration)
	float RandomFloatBetween0and1();

	UFUNCTION(BluePrintCallable, Category = LevelGeneration)
	virtual void ChangeGridViaAlgorithym();
	
	virtual void BeginPlay() override;

	virtual void CreateStaticMeshes();
};
