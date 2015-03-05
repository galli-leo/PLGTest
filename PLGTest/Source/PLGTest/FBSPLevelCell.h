#pragma once
#include "Math/Vector2D.h"
#include "FLevelCell.h"
#include "BSPLevelCell.h"


struct FBSPLevelCell : FLevelCell{

	
	FORCEINLINE FBSPLevelCell();



	FORCEINLINE FVector vectorForGridCoordinate(FVector2D Coordinate);

	FORCEINLINE void createStaticMeshWithGrid(TArray<bool>floors, UClass *lightClass);
	FVector center;

	//FString ASDFRectDescription();

};



FORCEINLINE FBSPLevelCell::FBSPLevelCell()
{

}

FORCEINLINE FVector FBSPLevelCell::vectorForGridCoordinate(FVector2D Coordinate)
{
	
		FVector vector;
		vector.Y = center.Y + Coordinate.X * 192.0f;
		vector.X = center.X + Coordinate.Y * 192.0f;
		vector.Z = center.Z;
		return vector;
	
}

template <typename VictoryObjType>
static FORCEINLINE VictoryObjType* SpawnBP(
	UWorld* TheWorld,
	UClass* TheBP,
	const FVector& Loc,
	const FRotator& Rot,
	const bool bNoCollisionFail = true,
	AActor* Owner = NULL,
	APawn* Instigator = NULL
	){
	if (!TheWorld) return NULL;
	if (!TheBP) return NULL;
	//~~

	FActorSpawnParameters SpawnInfo;
	SpawnInfo.bNoCollisionFail = bNoCollisionFail;
	SpawnInfo.Owner = Owner;
	SpawnInfo.Instigator = Instigator;
	SpawnInfo.bDeferConstruction = false;

	return TheWorld->SpawnActor<VictoryObjType>(TheBP, Loc, Rot, SpawnInfo);
}

FORCEINLINE void FBSPLevelCell::createStaticMeshWithGrid(TArray<bool>floors, UClass *lightClass)
{
	UWorld *TheWorld = GEngine->GetWorld();
	int32 x = this->GridCoordinate.X;
	int32 y = this->GridCoordinate.Y;
	float randomZ = floor(((double)rand() / (RAND_MAX)) *(396.0f / 1.8 - 396.0f / 2.1 + 1.0f) + 396.0f / 2.1);
	float randomX = floor(((double)rand() / (RAND_MAX)) *(192.0f / 1.5 - 192.0f / 2.5 + 1.0f) + 192.0f / 2.5);
	bool should = false;
	
	if (x % 2 == 0 && y % 2 == 0)
	{
		should = true;
	}
	if (x % 2 == 1 && y % 2 == 1)
	{
		should = true;
	}


	if (should)
	{
		if (x != 0)
		{


			if (floors[0])
			{


				FVector Loc = FVector(vectorForGridCoordinate(FVector2D(x,y)));
				FRotator Rot = FRotator(0, -90, 0);


				GEngine->AddOnScreenDebugMessage(-7, 10, FColor::Red, "Hello");
				AActor * lightActor = SpawnBP<AActor>(TheWorld, lightClass, Loc, Rot);
				
			}
		}

		if (x != 79)
		{


			if (floors[1])
			{


				FVector Loc = FVector(vectorForGridCoordinate(FVector2D(x, y)));
				FRotator Rot = FRotator(0, -270, 0);



				AActor * lightActor = SpawnBP<AActor>(TheWorld, lightClass, Loc, Rot);
				
			}
		}
		if (y != 0)
		{


			if (floors[2])
			{


				FVector Loc = FVector(vectorForGridCoordinate(FVector2D(x, y)));
				FRotator Rot = FRotator(0, 180, 0);



				AActor * lightActor = SpawnBP<AActor>(TheWorld, lightClass, Loc, Rot);
				
			}
		}

		if (y != 79)
		{


			if (floors[3])
			{


				FVector Loc = FVector(vectorForGridCoordinate(FVector2D(x, y)));
				FRotator Rot = FRotator(0, 0, 0);



				AActor * lightActor = SpawnBP<AActor>(TheWorld, lightClass, Loc, Rot);
				
			}
		}
	}
}


