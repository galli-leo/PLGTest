

#pragma once

#include "LevelCell.h"
#include "BSPGridRow.h"
#include "BSPLevelCell.generated.h"

/**
 * 
 */
UCLASS()
class ABSPLevelCell : public ALevelCell
{
	GENERATED_UCLASS_BODY()
		
	void createStaticMeshWithGrid(TArray<UBSPGridRow> grid);
	
	UClass *lightClass;

	TSubobjectPtr<UPointLightComponent> lightLeft;
};


