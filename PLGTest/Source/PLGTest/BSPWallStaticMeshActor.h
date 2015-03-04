

#pragma once

#include "Engine/StaticMeshActor.h"
#include "BSPWallStaticMeshActor.generated.h"

/**
 * 
 */
UCLASS()
class ABSPWallStaticMeshActor : public AStaticMeshActor
{
	GENERATED_UCLASS_BODY()
	
	UPROPERTY()
	TSubobjectPtr<UInstancedStaticMeshComponent> InstancedStaticMeshComponent;


	UPROPERTY()
	UStaticMesh* SMAsset_Cube;
	
	
};
