

#include "PLGTest.h"
#include "LevelCell.h"


ALevelCell::ALevelCell(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	DrawSphereComponent = PCIP.CreateDefaultSubobject<USphereComponent>(this, FName("DrawSphereComponent0"));

	if (DrawSphereComponent)
	{
		DrawSphereComponent->ShapeColor.R = 255;
		DrawSphereComponent->ShapeColor.G = 0;
		DrawSphereComponent->ShapeColor.B = 0;
		DrawSphereComponent->ShapeColor.A = 255;


		DrawSphereComponent->bAbsoluteScale = true;
	}
	//Components.Add(DrawSphereComponent);
	RootComponent = DrawSphereComponent;

	Mesh = PCIP.CreateDefaultSubobject<UStaticMeshComponent>(this, FName("Mesh"));
	
	Mesh->AttachTo(RootComponent);

}
void ALevelCell::BeginPlay()
{
	
	FString string = FString::SanitizeFloat(this->GetActorLocation().X);
	GEngine->AddOnScreenDebugMessage(-2, 5.f, FColor::Red, string);

}
void ALevelCell::createStaticMeshWithMeshes(UStaticMesh* wallMesh, UStaticMesh* floorMesh)
{
	if (this->Type == LevelCellTypeWall && wallMesh != NULL){
		Mesh->SetStaticMesh(wallMesh);
	}
	else if (this->Type == LevelCellTypeFloor && floorMesh != NULL){
		//Mesh->SetStaticMesh(floorMesh);
	}
}
