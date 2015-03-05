

#pragma once
#include "FRect.h"
#include "BSPRoomArray.h"
#include "LevelGenerator.h"
#include "BSPGridRow.h"

#include "BSPLevelGenerator.generated.h"

/**
 * 
 */
UCLASS()
class ABSPLevelGenerator : public ALevelGenerator
{
	GENERATED_UCLASS_BODY()

	enum SplitDirection
	{
		SplitDirectionHorizontal,
		SplitDirectionVertical
	};
	TArray<UBSPGridRow> grid;
	TArray<BSPRoomArray> roomArray;
	TArray<FRect> finalRoomArray;
	UClass *ceilingLightClass;
	UClass *exitClass;
	UClass *startClass;
	UClass *lightClass;
	int8 loops;

	virtual void ChangeGridViaAlgorithym() override;
	
	TArray<FRect> SplitRoomRandomly(FRect room, SplitDirection previousSplitDirection, int32 stage);

	void FillOutRoom(FRect room);

	UFUNCTION(BluePrintCallable,Category = LevelGeneration )
	int32 RandomIntBetweenFloats(float float1, float float2);
	
	virtual void GenerateLevelWithSeed(int32 seed) override;

	FVector2D RandomPositionInRect(FRect rect);

	void MakePathBetweenCoords(FVector2D start, FVector2D end);

	virtual void InitializeArray(UWorld *world) override;

	virtual void CreateStaticMeshes();

	virtual FBSPLevelCell* LevelCellForGridCoordinate(FVector2D Coordinate);

	void CreateWallLight(FBSPLevelCell *cell);

	UFUNCTION(BluePrintCallable, Category = Other)
	int32 getSeedForString(FString seed);

	UFUNCTION(BluePrintCallable, Category = Other)
		void openMap();
};

