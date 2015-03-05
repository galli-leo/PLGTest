

#include "PLGTest.h"
#include "LevelGenerator.h"


ALevelGenerator::ALevelGenerator(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	//Just to get something in the editor
	DrawSphereComponent = PCIP.CreateDefaultSubobject<USphereComponent>(this, FName("DrawSphereComponent0"));
	
	

	if (DrawSphereComponent)
	{
		DrawSphereComponent->ShapeColor.R = 173;
		DrawSphereComponent->ShapeColor.G = 239;
		DrawSphereComponent->ShapeColor.B = 231;
		DrawSphereComponent->ShapeColor.A = 255;

		
		DrawSphereComponent->bAbsoluteScale = true;
	}
	
	RootComponent = DrawSphereComponent;

	//Get the StaticMesh for the wall
	static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticMeshOb_AW3(TEXT("StaticMesh'/Game/Shapes/Shape_Cube.Shape_Cube'"));
	if (StaticMeshOb_AW3.Object)
	{
		wallMesh = StaticMeshOb_AW3.Object;
	}

	//Initialize some default variables
	GridSize = FSize(64, 64);
	TileSize = FSize(100, 100);
}
void ALevelGenerator::GenerateLevelWithSeed(int32 seed)
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Level Generation Started!");
	
	
	//UWorld*  World = GetWorld();
	//const float startTime = World->GetTimeSeconds();
	//srand(seed);
	//InitializeArray(GetWorld());
	
	//ChangeGridViaAlgorithym();

	//const float endTime = World->GetTimeSeconds();
	//float time = endTime - startTime;
	
	IsValidGridCoordinate(FVector2D(0, 0));
//	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Level Generation Finished in "+FString::SanitizeFloat(time)+" Secconds!");

	//this->SetActorRotation(FRotator(0,45,0));
	
}
bool ALevelGenerator::IsValidGridCoordinate(FVector2D Coordinate)
{
	if (Coordinate.X >= 0 && Coordinate.X < GridSize.width && Coordinate.Y >= 0 && Coordinate.Y < GridSize.height)
	{
		return true;
	}

	return false;
}
FLevelCell* ALevelGenerator::LevelCellForGridCoordinate(FVector2D Coordinate)
{
	if (IsValidGridCoordinate(Coordinate))
	{
		FLevelCell* cell = grid[Coordinate.X].columns[Coordinate.Y];
		return cell;
	}
	return new FLevelCell();
}
FVector ALevelGenerator::VectorForGridCoordinate(FVector2D Coordinate)
{
	if (IsValidGridCoordinate(Coordinate))
	{
		FVector vector;
		vector.Y = this->GetActorLocation().Y + Coordinate.X * 192.0f;
		vector.X = this->GetActorLocation().X + Coordinate.Y * 192.0f;
		vector.Z = this->GetActorLocation().Z;
		return vector;
	}
	return FVector::ZeroVector;
}

void ALevelGenerator::BeginPlay()
{
	
	//GenerateLevelWithSeed(0);
}
void ALevelGenerator::CreateStaticMeshes()
{

		for (int32 x = 0; x < GridSize.width; x++)
		{


			for (int32 y = 0; y <GridSize.height; y++)
			{
				
				
				LevelCellForGridCoordinate(FVector2D(x,y))->createStaticMeshWithMeshes(wallMesh, NULL);
				
		
			}

		}
	
}
void ALevelGenerator::InitializeArray(UWorld* world)
{
	
	for (int32 x = 0; x < GridSize.width; x++)
	{

		grid.Add(UGridRow());
		for (int32 y = 0; y < GridSize.height; y++)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = Instigator;

			FVector SpawnVector = VectorForGridCoordinate(FVector2D(x, y));

			FLevelCell* cell = new FLevelCell();
			
			double random = ((double)rand() / (RAND_MAX));
			cell->Type = FLevelCell::LevelCellTypeWall;
			
			grid[x].columns.Add(cell);
		}

	}
}

float ALevelGenerator::RandomFloatBetween0and1()
{
	return ((double)rand() / (RAND_MAX));
}

void ALevelGenerator::ChangeGridViaAlgorithym()
{
	//Implement Algorithym here in subclass
}