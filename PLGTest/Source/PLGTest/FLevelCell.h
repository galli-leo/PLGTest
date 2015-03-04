#pragma once
#include "Math/Vector2D.h"
struct FLevelCell{
	
	enum FLevelCellType
	{
		LevelCellTypeInvalid = -1,
		LevelCellTypeWall,
		LevelCellTypeFloor,
		LevelCellTypeOther
	};
	FLevelCellType Type;
	FVector2D GridCoordinate;
	/*FString Description()
	{
	return "width: " + FString::SanitizeFloat(width) + ", height: " + FString::SanitizeFloat(height);
	}*/
	FORCEINLINE FLevelCell();
	void createStaticMeshWithMeshes(UStaticMesh* wallMesh, UStaticMesh* floorMesh);
	

	//FString ASDFRectDescription();

};

FORCEINLINE FLevelCell::FLevelCell()
{

}

FORCEINLINE void FLevelCell::createStaticMeshWithMeshes(UStaticMesh* wallMesh, UStaticMesh* floorMesh)
{

}


