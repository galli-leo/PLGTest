

#include "PLGTest.h"
//#include "FRect.h"
#include "BSPLevelCell.h"
#include "LevelCell.h"
#include "BSPWallStaticMeshActor.h"
#include "BSPLevelGenerator.h"


ABSPLevelGenerator::ABSPLevelGenerator(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	//Find Wall Mesh
	static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticMeshOb_AW3(TEXT("StaticMesh'/Game/Props/BSPWall.BSPWall'"));
	if (StaticMeshOb_AW3.Object)
	{
		wallMesh = StaticMeshOb_AW3.Object;
	}
	//Set how many times the rooms are divided (5 is good)
	loops = 5;
	//Set the GridSize (80, 80 seems good)
	GridSize = FSize(80, 80);
	//Set the size of one tile (size of the static mes)
	TileSize = FSize(192, 192);
	//Find the ceiling Blueprint
	static ConstructorHelpers::FObjectFinder<UBlueprint> ItemBlueprint(TEXT("Blueprint'/Game/Blueprints/CeilingLight.CeilingLight'"));
	if (ItemBlueprint.Object){
		ceilingLightClass = (UClass*)ItemBlueprint.Object->GeneratedClass;

	}
	//Find the Exit Blueprint
	static ConstructorHelpers::FObjectFinder<UBlueprint> ExitBlueprint(TEXT("Blueprint'/Game/Blueprints/exit.exit'"));
	if (ExitBlueprint.Object){
		exitClass = (UClass*)ExitBlueprint.Object->GeneratedClass;

	}
	//Find the Start Blueprint
	static ConstructorHelpers::FObjectFinder<UBlueprint> StartBlueprint(TEXT("Blueprint'/Game/Blueprints/playerStart.playerStart'"));
	if (StartBlueprint.Object){
		startClass = (UClass*)StartBlueprint.Object->GeneratedClass;

	}
	//Find the Wall light Blueprint
	static ConstructorHelpers::FObjectFinder<UBlueprint> LightBlueprint(TEXT("Blueprint'/Game/Blueprints/BSPWallLight.BSPWallLight'"));
	if (LightBlueprint.Object){
		lightClass = (UClass*)LightBlueprint.Object->GeneratedClass;

	}
	this->RootComponent->SetMobility(EComponentMobility::Static);
	
	
}

void ABSPLevelGenerator::ChangeGridViaAlgorithym()
{
	SplitDirection previousSplitDirection = SplitDirectionVertical;
	for (int32 stage = 0; stage < loops; stage++)
	{
		
		roomArray.Add(BSPRoomArray());
		//If this is the first stage we need to initialize the first room
		if (stage == 0)
		{
			//first stage
			FRect firstRoom = FRect(GridSize.width, GridSize.height, 0, 0);
			TArray<FRect> newRooms = SplitRoomRandomly(firstRoom, previousSplitDirection, stage);
			roomArray[stage].rooms.Add(newRooms[0]);
			roomArray[stage].rooms.Add(newRooms[1]);
		}
		else
		{
			//take the rooms from the previous stage and split them randomly to create this stage
			for (int32 i = 0; i < roomArray[stage - 1].rooms.Num(); i++)
			{
				FRect room = roomArray[stage - 1].rooms[i];
				TArray<FRect> newRooms = SplitRoomRandomly(room, previousSplitDirection, stage);
				roomArray[stage].rooms.Add(newRooms[0]);
				roomArray[stage].rooms.Add(newRooms[1]);
			}
		}
		//Switch split direction
		if (previousSplitDirection == SplitDirectionHorizontal)
		{
			previousSplitDirection = SplitDirectionVertical;
		}
		else
		{
			previousSplitDirection = SplitDirectionHorizontal;
		}

	}
	FString theString = "";
	//Loop through the final stage to get the final rooms, then randomly determine the size of the room
	for (int32 j = 0; j < roomArray[loops-1].rooms.Num(); j++)
	{
		FRect room = roomArray[loops - 1].rooms[j];
		float randomHeight = RandomIntBetweenFloats(3.0f, room.height - 1);
		float randomWidth = RandomIntBetweenFloats(3.0f, room.width - 1);
		if (randomHeight/randomWidth >3.5)
		{
			randomHeight = RandomIntBetweenFloats(3.0f, room.height/1.7f - 1);
		}
		else if (randomWidth / randomHeight >3.5)
		{
			randomWidth = RandomIntBetweenFloats(3.0f, room.width/ 1.7f - 1);
		}
		
		float maxX = room.x + room.width-1-randomWidth;
		float maxY = room.y + room.height-1
			- randomHeight;
		float randomX = RandomIntBetweenFloats(room.x, maxX);
		float randomY = RandomIntBetweenFloats(room.y, maxY);

		FRect finalRoom = FRect(randomWidth, randomHeight, randomX, randomY);
		//This array is filled with the final hollow rooms.
		finalRoomArray.Add(finalRoom);
		//FRect finalRoom = FRect(room.width - 2, room.height - 2, room.x + 1, room.y + 1);
		FillOutRoom(finalRoom);
		//just some debug stuff
		theString = theString + FString::SanitizeFloat(finalRoom.width) + "," + FString::SanitizeFloat(finalRoom.height) + "," + FString::SanitizeFloat(finalRoom.x) + "," + FString::SanitizeFloat(finalRoom.y) + "::";
		//GEngine->AddOnScreenDebugMessage(-9, 5.f, FColor::Red, theString);
	}

	


	//Make the connections
	for (int32 k = 0; k < roomArray[loops - 1].rooms.Num(); k++)
	{
		//first one connects to nothing
		if (k==0)
		{

		}
		else
		{
			FRect room = roomArray[loops - 1].rooms[k];
			//get the last room
			FRect lastRoom = roomArray[loops - 1].rooms[k - 1];
			bool isCoordinate1Floor = true;
			bool isCoordinate2Floor = true;
			FVector2D startCoordinate = FVector2D();
			FVector2D destinationCoordinate = FVector2D();

			while (isCoordinate1Floor)
			{
				startCoordinate = RandomPositionInRect(room);
				if (LevelCellForGridCoordinate(startCoordinate)->Type==FLevelCell::LevelCellTypeFloor)
				{
					isCoordinate1Floor = false;
				}
			}

			while (isCoordinate2Floor)
			{
				destinationCoordinate = RandomPositionInRect(lastRoom);
				if (LevelCellForGridCoordinate(destinationCoordinate)->Type == FLevelCell::LevelCellTypeFloor)
				{
					isCoordinate2Floor = false;
				}
			}

			MakePathBetweenCoords(startCoordinate, destinationCoordinate);
		}
	}
	
	/*Set the walls*/
	for (int32 x = 0; x < GridSize.width; x++)
	{
		for (int32 y = 0; y < GridSize.height; y++)
		{
			if (x == 0 || y == 0 /*|| x == GridSize.width - 1 || GridSize.height - 1*/)
			{
				grid[x].columns[y]->Type = FLevelCell::LevelCellTypeWall;
			}
		}

	}

	//Loop through the final hollow rooms, populate them, if they are big we need some lights
	for (int32 index = 0; index < finalRoomArray.Num(); index++)
	{
		FRect finalRoom = finalRoomArray[index];
		if (finalRoom.height>3 && finalRoom.width>3)
		{
			//WE need some lights

			for (int32 x = 0; x < finalRoom.width-3; x++)
			{
				if (x%3==0)
				{
					for (int32 y = 0; y < finalRoom.height - 3; y++)
					{
						if (y % 3 == 0)
						{
							FVector2D gridCoordinate = FVector2D(finalRoom.x + 1 + x, finalRoom.y + 1 + y);
							FVector spawnVector = VectorForGridCoordinate(gridCoordinate);
							spawnVector.Z = spawnVector.Z + 400.0f;

							FActorSpawnParameters SpawnInfo;

							//SET NO COLLISION FAIL TO TRUE
							SpawnInfo.bNoCollisionFail = true;

							SpawnInfo.Owner = this;
							AActor *ceilingLightActor = SpawnBP<AActor>(GetWorld(), ceilingLightClass, spawnVector, FRotator(0, 0, 0));
							ceilingLightActor->AttachRootComponentToActor(this);
						}

					}
				}
			}
		}
	}

	//Choose Start and exit
	//NEEDS FIXING SPAWN WAY TO CLOSE
	/*FVector2D randomStart;
	FVector2D randomExit;
	bool isRandomStartGood = false;
	bool isRandomExitGood = false;
	while (!isRandomStartGood)
	{
		int32 randomX = RandomIntBetweenFloats(GridSize.width-1, 0.0);
		int32 randomY = RandomIntBetweenFloats(GridSize.height-1, 0.0);
		if (LevelCellForGridCoordinate(FVector2D(randomX,randomY))->Type == FLevelCell::LevelCellTypeFloor)
		{
			randomStart = FVector2D(randomX, randomY);
			isRandomStartGood = true;
		}
	}
	FVector vect = VectorForGridCoordinate(randomStart);
	vect.X = vect.X + 96.0;
	vect.Y = vect.Y + 96.0;
	
	AActor *playerStart = SpawnBP<AActor>(GetWorld(), startClass, vect, FRotator(0, 0, 0));
	playerStart->AttachRootComponentToActor(this);
	while (!isRandomExitGood)
	{
		int32 randomX = RandomIntBetweenFloats(GridSize.width-1, 0.0);
		int32 randomY = RandomIntBetweenFloats(GridSize.height-1, 0.0);
		if (LevelCellForGridCoordinate(FVector2D(randomX, randomY))->Type == FLevelCell::LevelCellTypeFloor && (randomExit-randomStart).Size()>40)
		{
			randomExit = FVector2D(randomX, randomY);
			isRandomExitGood = true;
		}
	}
	
	
	FVector spawnVector = VectorForGridCoordinate(randomExit);
	

	FActorSpawnParameters SpawnInfo;

	//SET NO COLLISION FAIL TO TRUE
	SpawnInfo.bNoCollisionFail = true;

	SpawnInfo.Owner = this;
	AActor *exitActor = SpawnBP<AActor>(GetWorld(), exitClass, spawnVector, FRotator(0, 0, 0));
	exitActor->AttachRootComponentToActor(this);*/
	

}

TArray<FRect> ABSPLevelGenerator::SplitRoomRandomly(FRect room, SplitDirection previousSplitDirection, int32 stage)
{
	FRect room1 = FRect();
	FRect room2 = FRect();
	//Check wich way to split
	if (previousSplitDirection == SplitDirectionHorizontal)
	{
		//some numbers for the random generator
		float maxHeight = room.y + room.height;
		float splitPoint = 0.0f;
		bool isSplitPointGood = true;
		
		//some numbers to determine the perfect middle and then the max and min split points, so we dont get really small rooms TODO DOC
		int32 rootNum = stage / 2;
		rootNum++;
		int32 perfectMiddle = GridSize.height / pow(2.0f, rootNum);
		int32 minSplitPoint = perfectMiddle / 2 + room.y;
		int32 maxSplitPoint = minSplitPoint + perfectMiddle*1.5;
		int32 count = 0;
		while (isSplitPointGood)
		{
			splitPoint = floor((RandomFloatBetween0and1()*(maxHeight - room.y + 1.0f)) + room.y);
			if (splitPoint >= minSplitPoint && splitPoint <= maxSplitPoint && maxHeight-splitPoint>perfectMiddle/2)
			{
				isSplitPointGood = false;
				GEngine->AddOnScreenDebugMessage(-7, 5.f, FColor::Red, "isgood");
			}
			GEngine->AddOnScreenDebugMessage(-8, 5.f, FColor::Red, FString::SanitizeFloat(splitPoint));
			if (count == 100)
			{
				splitPoint = room.x + perfectMiddle / 2.0f;
				isSplitPointGood = false;
			}
			count++;
		}

		GEngine->AddOnScreenDebugMessage(-6, 5.f, FColor::Red, FString::FromInt(perfectMiddle) + "," + FString::FromInt(minSplitPoint) + "," + FString::FromInt(maxSplitPoint) + "," + FString::FromInt(splitPoint) + "," + FString::FromInt(rootNum)+"," + FString::FromInt(floor(pow(64.0f, (1/2)))));
		 room1 = FRect(room.width, splitPoint - room.y, room.x, room.y);
		 room2 = FRect(room.width, maxHeight - splitPoint, room.x, splitPoint);
		GEngine->AddOnScreenDebugMessage(-4, 5.f, FColor::Red, "Room1:" + FString::SanitizeFloat(room1.width) + "," + FString::SanitizeFloat(room1.height) + "," + FString::SanitizeFloat(room1.x) + "," + FString::SanitizeFloat(room1.y));
		GEngine->AddOnScreenDebugMessage(-5, 5.f, FColor::Red, "Room2:" + FString::SanitizeFloat(room1.width) + "," + FString::SanitizeFloat(room1.height) + "," + FString::SanitizeFloat(room1.x) + "," + FString::SanitizeFloat(room1.y));
	}
	else
	{
		float maxHeight = room.x + room.width;
		float splitPoint = 0.0f;
		bool isSplitPointGood2 = true;

		stage++;
		int32 rootNum = stage / 2;
		rootNum++;
		int32 perfectMiddle = GridSize.width / pow(2.0f, rootNum);
		int32 minSplitPoint = perfectMiddle / 2 + room.x;
		int32 maxSplitPoint = minSplitPoint + perfectMiddle*1.5;
		int32 count = 0;

		while (isSplitPointGood2)
		{
			splitPoint = floor((RandomFloatBetween0and1()*(maxHeight - room.x + 1.0f)) + room.x);
			if (splitPoint >= minSplitPoint && splitPoint <= maxSplitPoint && maxHeight - splitPoint>perfectMiddle / 2)
			{
				isSplitPointGood2 = false;
				GEngine->AddOnScreenDebugMessage(-7, 5.f, FColor::Red, "isgood");
			}
			GEngine->AddOnScreenDebugMessage(-8, 5.f, FColor::Red, FString::SanitizeFloat(splitPoint));
			if (count == 100)
			{
				splitPoint = room.x + perfectMiddle / 2.0f;
				isSplitPointGood2 = false;
			}
			count++;
		}
		GEngine->AddOnScreenDebugMessage(-6, 5.f, FColor::Red, FString::FromInt(perfectMiddle) + "," + FString::FromInt(minSplitPoint) + "," + FString::FromInt(maxSplitPoint) + "," + FString::FromInt(splitPoint) + "," + FString::FromInt(rootNum) + "," + FString::FromInt(floor(pow(64.0f, (1 / 2)))));
		room1 = FRect(splitPoint - room.x, room.height, room.x, room.y);
		room2 = FRect(maxHeight - splitPoint, room.height, splitPoint, room.y);
		GEngine->AddOnScreenDebugMessage(-4, 5.f, FColor::Red, "Room1:" + FString::SanitizeFloat(room1.width) + "," + FString::SanitizeFloat(room1.height) + "," + FString::SanitizeFloat(room1.x) + "," + FString::SanitizeFloat(room1.y));
		GEngine->AddOnScreenDebugMessage(-5, 5.f, FColor::Red, "Room2:" + FString::SanitizeFloat(room1.width) + "," + FString::SanitizeFloat(room1.height) + "," + FString::SanitizeFloat(room1.x) + "," + FString::SanitizeFloat(room1.y));
	}
	TArray<FRect> endRooms = TArray<FRect>();
	endRooms.Add(room1);
	endRooms.Add(room2);
	return endRooms;
}

//Make the given set of coordinates floor cells
void ABSPLevelGenerator::FillOutRoom(FRect room)
{
	for (int32 x = room.x; x < room.width+room.x; x++)
	{
		for (int32 y = room.y; y < room.height+room.y; y++)
		{
			grid[x].columns[y]->Type = FLevelCell::LevelCellTypeFloor;
		}
	}
}

int32 ABSPLevelGenerator::RandomIntBetweenFloats(float float1, float float2)
{
	return floor(RandomFloatBetween0and1()*(float2 - float1 + 1.0f) + float1);
}

FVector2D ABSPLevelGenerator::RandomPositionInRect(FRect rect)
{
	float maxX = rect.x + rect.width;
	float maxY = rect.y + rect.height;
	return FVector2D(RandomIntBetweenFloats(rect.x, maxX-1), RandomIntBetweenFloats(rect.y, maxY-1));
}

void ABSPLevelGenerator::MakePathBetweenCoords(FVector2D start, FVector2D end)
{

	if (start.X > end.X)
	{
		for ( float x = start.X;  x >= end.X;  x--)
		{
			LevelCellForGridCoordinate(FVector2D(x, start.Y))->Type = FLevelCell::LevelCellTypeFloor;
		}
	}
	else
	{
		for (float x = start.X; x <= end.X; x++)
		{
			LevelCellForGridCoordinate(FVector2D(x, start.Y))->Type = FLevelCell::LevelCellTypeFloor;
		}
	}
	if (start.Y > end.Y)
	{
		for (float y = start.Y; y >= end.Y; y--)
		{
			LevelCellForGridCoordinate(FVector2D(end.X, y))->Type = FLevelCell::LevelCellTypeFloor;
		}
	}
	else
	{
		for (float y = start.Y; y <= end.Y; y++)
		{
			LevelCellForGridCoordinate(FVector2D(end.X, y))->Type = FLevelCell::LevelCellTypeFloor;
		}
	}
}

//Set everything default to wall
void ABSPLevelGenerator::InitializeArray(UWorld *world)
{
	for (int32 x = 0; x < GridSize.width; x++)
	{

		grid.Add(UBSPGridRow());
		for (int32 y = 0; y < GridSize.height; y++)
		{
			

			FBSPLevelCell* cell = new FBSPLevelCell();
			//UGameplayStatics::FinishSpawningActor(cell, FTransform());
			//double random = ((double)rand() / (RAND_MAX));
			cell->Type = FBSPLevelCell::LevelCellTypeWall;
			cell->GridCoordinate = FVector2D(x, y);
			
			grid[x].columns.Add(cell);
		}

	}
}

//finally create the static meshes
void ABSPLevelGenerator::CreateStaticMeshes()
{
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.Owner = this;

	//SET NO COLLISION FAIL TO TRUE
	SpawnInfo.bNoCollisionFail = true;

	//SpawnInfo.Owner = this;

	ABSPWallStaticMeshActor* wallActor =
		GetWorld()->SpawnActor<ABSPWallStaticMeshActor>(
		ABSPWallStaticMeshActor::StaticClass(),
		this->GetActorLocation(),
		FRotator::ZeroRotator,
		SpawnInfo
		);
	if (!wallActor)                    return;
	if (!wallActor->InstancedStaticMeshComponent)   return;
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	//Mesh
	wallActor->InstancedStaticMeshComponent->SetStaticMesh(wallMesh);
	//wallActor->AttachRootComponentToActor(this);
	//Add Core Instance
	/*FTransform newT = NewVertex->GetTransform();
	newT.SetLocation(FVector(0, 0, 0));
	NewVertex->InstancedStaticMeshComponent->AddInstance(newT);*/

	//Scale
	//wallActor->SetActorRelativeScale3D(this->GetActorScale3D());
	

	for (int32 x = 0; x < GridSize.width; x++)
	{


		for (int32 y = 0; y <GridSize.height; y++)
		{

			if (LevelCellForGridCoordinate(FVector2D(x, y))->Type == FLevelCell::LevelCellTypeWall)
			{


				FTransform newT = wallActor->GetTransform();
				newT.SetLocation(VectorForGridCoordinate(FVector2D(x, y)));
				//if (x == 0 && y < 3)
				wallActor->InstancedStaticMeshComponent->AddInstance(newT);
				//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Test2");

				FBSPLevelCell* celli = LevelCellForGridCoordinate(FVector2D(x, y));
				//GEngine->AddOnScreenDebugMessage(-2, 5.f, FColor::Red, "Test3");
				//Array for which tiles are floors in this order: W, E, N, S



				celli->center = this->GetActorLocation();
				//celli->createStaticMeshWithGrid(floors, lightClass);
				//Create wall lights
				CreateWallLight(celli);
			}

		}

	}
}

FBSPLevelCell* ABSPLevelGenerator::LevelCellForGridCoordinate(FVector2D Coordinate)
{
	if (IsValidGridCoordinate(Coordinate))
	{
		FBSPLevelCell* cell = grid[Coordinate.X].columns[Coordinate.Y];
		return cell;
	}
	return NULL;
}

//Turn string into a number
int32 ABSPLevelGenerator::getSeedForString(FString seed)
{

	 char* String[300];
	for (int32 i = 0; i < seed.GetCharArray().Num(); i++)
	{
		String[i] = (char*)seed.GetCharArray()[i];
	}
	int32 seed2 = 0;
	for (int32 j = 0; j < seed.GetCharArray().Num(); j++)
	{
		seed2 += (int32)String[j];
	}
	
	return seed2;
}
void ABSPLevelGenerator::openMap()
{
	
	FURL url = FURL();
	url.Map = "E:\\Copy\\Unreal Projects\\PLGTest\\Content\\test.umap";
	//url.Protocol = "unreal";
	FWorldContext context = FWorldContext();
	context.SetCurrentWorld(GetWorld());
	FString error;
	GetWorld()->CleanupWorld();
	GetWorld()->ForceGarbageCollection();
	GetWorld()->RemoveFromRoot();
	UGameplayStatics::OpenLevel(GetWorld(), "test", true);
	//GEngine->LoadMap(context, url, nullptr, error);
}

void ABSPLevelGenerator::GenerateLevelWithSeed(int32 seed)
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Level Generation Started!");


	//UWorld*  World = GetWorld();
	//const float startTime = World->GetTimeSeconds();
	srand(seed);
	InitializeArray(GetWorld());

	ChangeGridViaAlgorithym();

	//const float endTime = World->GetTimeSeconds();
	//float time = endTime - startTime;
	CreateStaticMeshes();
	IsValidGridCoordinate(FVector2D(0, 0));
	//	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Level Generation Finished in "+FString::SanitizeFloat(time)+" Secconds!");

	//this->SetActorRotation(FRotator(0,45,0));

}

void ABSPLevelGenerator::CreateWallLight(FBSPLevelCell *cell)
{
	int32 x = cell->GridCoordinate.X;
	int32 y = cell->GridCoordinate.Y;
	UWorld *TheWorld = GetWorld();
	TArray<bool> floors;

	for (int32 i = 0; i < 6; i++)
	{
		floors.Add(false);
	}
	if (x != 0)
	{
		floors[0] = (LevelCellForGridCoordinate(FVector2D(x - 1, y))->Type == FLevelCell::LevelCellTypeFloor);
	}

	if (x != 79)
	{
		floors[1] = (LevelCellForGridCoordinate(FVector2D(x + 1, y))->Type == FLevelCell::LevelCellTypeFloor);
	}

	if (y != 0)
	{
		floors[2] = (LevelCellForGridCoordinate(FVector2D(x, y - 1))->Type == FLevelCell::LevelCellTypeFloor);
	}

	if (y != 79)
	{
		floors[3] = (LevelCellForGridCoordinate(FVector2D(x, y + 1))->Type == FLevelCell::LevelCellTypeFloor);
	}
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


				FVector Loc = FVector(VectorForGridCoordinate(FVector2D(x, y)));
				Loc.X += randomX;
				Loc.Z += randomZ;
				Loc.Y -= 192.0f;
				FRotator Rot = FRotator(0, -90, 0);


				GEngine->AddOnScreenDebugMessage(-7, 10, FColor::Red, "Hello");
				AActor * lightActor = SpawnBP<AActor>(TheWorld, lightClass, Loc, Rot);

			}
		}

		if (x != 79)
		{


			if (floors[1])
			{


				FVector Loc = FVector(VectorForGridCoordinate(FVector2D(x, y)));
				FRotator Rot = FRotator(0, -270, 0);
				Loc.X += randomX;
				Loc.Z += randomZ;


				AActor * lightActor = SpawnBP<AActor>(TheWorld, lightClass, Loc, Rot);

			}
		}
		if (y != 0)
		{


			if (floors[2])
			{


				FVector Loc = FVector(VectorForGridCoordinate(FVector2D(x, y)));
				FRotator Rot = FRotator(0, 180, 0);
				Loc.Y -= randomX;
				Loc.Z += randomZ;
				


				AActor * lightActor = SpawnBP<AActor>(TheWorld, lightClass, Loc, Rot);

			}
		}

		if (y != 79)
		{


			if (floors[3])
			{


				FVector Loc = FVector(VectorForGridCoordinate(FVector2D(x, y)));
				FRotator Rot = FRotator(0, 0, 0);
				Loc.Y -= randomX;
				Loc.Z += randomZ;
				Loc.X += 192.0f;

				AActor * lightActor = SpawnBP<AActor>(TheWorld, lightClass, Loc, Rot);

			}
		}
	}
}

