

#include "PLGTest.h"
#include "BSPLevelCell.h"


ABSPLevelCell::ABSPLevelCell(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{

	static ConstructorHelpers::FObjectFinder<UBlueprint> ItemBlueprint(TEXT("Blueprint'/Game/Blueprints/BSPWallLight.BSPWallLight'"));
	if (ItemBlueprint.Object){
		lightClass = (UClass*)ItemBlueprint.Object->GeneratedClass;
		
	}
	
	
}


void ABSPLevelCell::createStaticMeshWithGrid(TArray<UBSPGridRow> grid)
{
	/*UWorld *TheWorld = GetWorld();
	int32 x = this->GridCoordinate.X;
	int32 y = this->GridCoordinate.Y;
	float randomZ = floor(((double)rand() / (RAND_MAX)) *(396.0f / 1.8 - 396.0f / 2.1 + 1.0f) + 396.0f / 2.1);
	float randomX = floor(((double)rand() / (RAND_MAX)) *(192.0f / 1.5 - 192.0f / 2.5 + 1.0f) + 192.0f / 2.5);
	bool should = false;

	if (x%2==0 && y%2==0)
	{
		should = true;
	}
	if (x%2 ==1 && y%2== 1)
	{
		should = true;
	}
	
	
	if (should)
	{
		if (x != 0)
		{

			
			if (grid[x - 1].columns[this->GridCoordinate.Y].Type == ALevelCell::LevelCellTypeFloor)
			{


				FVector Loc = FVector(this->GetActorLocation().X + randomX, this->GetActorLocation().Y, this->GetActorLocation().Z + randomZ);
				FRotator Rot = FRotator(0, -90, 0);



				AActor * lightActor = SpawnBP<AActor>(TheWorld, lightClass, Loc, Rot);
				lightActor->AttachRootComponentToActor(this);
			}
		}

		if (x != 79)
		{


			if (grid[x + 1].columns[this->GridCoordinate.Y]->Type == ALevelCell::LevelCellTypeFloor)
			{


				FVector Loc = FVector(this->GetActorLocation().X + randomX, this->GetActorLocation().Y + 192.0f, this->GetActorLocation().Z + randomZ);
				FRotator Rot = FRotator(0, -270, 0);



				AActor * lightActor = SpawnBP<AActor>(TheWorld, lightClass, Loc, Rot);
				lightActor->AttachRootComponentToActor(this);
			}
		}
		if (y != 0)
		{


			if (grid[x].columns[this->GridCoordinate.Y - 1]->Type == ALevelCell::LevelCellTypeFloor)
			{


				FVector Loc = FVector(this->GetActorLocation().X, this->GetActorLocation().Y + randomX, this->GetActorLocation().Z + randomZ);
				FRotator Rot = FRotator(0, 180, 0);



				AActor * lightActor = SpawnBP<AActor>(TheWorld, lightClass, Loc, Rot);
				lightActor->AttachRootComponentToActor(this);
			}
		}

		if (y != 79)
		{


			if (grid[x].columns[this->GridCoordinate.Y + 1]->Type == ALevelCell::LevelCellTypeFloor)
			{


				FVector Loc = FVector(this->GetActorLocation().X + 192.0f, this->GetActorLocation().Y + randomX, this->GetActorLocation().Z + randomZ);
				FRotator Rot = FRotator(0, 0, 0);



				AActor * lightActor = SpawnBP<AActor>(TheWorld, lightClass, Loc, Rot);
				lightActor->AttachRootComponentToActor(this);
			}
		}
	}*/
}
