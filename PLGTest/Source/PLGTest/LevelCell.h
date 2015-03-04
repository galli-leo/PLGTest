

#pragma once

#include "GameFramework/Actor.h"
#include "LevelCell.generated.h"




/**
 * 
 */
UCLASS()


class ALevelCell : public AActor
{
	GENERATED_UCLASS_BODY()
	/*enum LevelCellType
	{
		LevelCellTypeInvalid = -1,
		LevelCellTypeWall,
		LevelCellTypeFloor,
		LevelCellTypeOther
	};*/
	UENUM(BlueprintType)       //"BlueprintType" is essential specifier

	//256 entries max
	enum LevelCellType
	{

		LevelCellTypeFloor    UMETA(DisplayName = "Floor"),
		LevelCellTypeWall     UMETA(DisplayName = "Wall"),
		LevelCellTypeInvalid  UMETA(DisplayName = "Invalid"),
		LevelCellTypeOther       UMETA(Hidden),
	};
	
	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = Lol)
	TEnumAsByte<LevelCellType> Type;
	FVector2D GridCoordinate;
	TSubobjectPtr<USphereComponent> DrawSphereComponent;
	
	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = StaticMeshComponents)
	TSubobjectPtr<UStaticMeshComponent> Mesh;

	virtual void BeginPlay() override;

	void createStaticMeshWithMeshes(UStaticMesh* wallMesh, UStaticMesh* floorMesh);

};
