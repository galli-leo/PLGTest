

#include "PLGTest.h"
#include "BSPWallStaticMeshActor.h"


ABSPWallStaticMeshActor::ABSPWallStaticMeshActor(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticMeshOb_AW3(TEXT("StaticMesh'/Game/Props/BSPWall.BSPWall'"));
	if (StaticMeshOb_AW3.Object)
	{
		SMAsset_Cube = StaticMeshOb_AW3.Object;
	}

	//create new object
	InstancedStaticMeshComponent = PCIP.CreateDefaultSubobject < UInstancedStaticMeshComponent >(this, TEXT("InstancedStaticMeshComponentCOMP"));
	RootComponent->SetMobility(EComponentMobility::Static);
	//Mobility
	StaticMeshComponent->SetMobility(EComponentMobility::Static);
	this->SetMobility(EComponentMobility::Static);
	//InstancedStaticMeshComponent->AttachTo(RootComponent);

	//Not Made some reason?
	if (!InstancedStaticMeshComponent) return;
	//~~~~~~~~~~~~~~~~~~~~~~~~

	//Set to Asset
	InstancedStaticMeshComponent->SetStaticMesh(SMAsset_Cube);

	InstancedStaticMeshComponent->bOwnerNoSee = false;
	InstancedStaticMeshComponent->SetMobility(EComponentMobility::Static);
	InstancedStaticMeshComponent->bCastDynamicShadow = false;
	InstancedStaticMeshComponent->CastShadow = false;
	
	//InstancedStaticMeshComponent->BodyInstance.OwnerComponent = RootComponent;
	
	//InstancedStaticMeshComponent->BodyInstance.SetObjectType(ECC_WorldStatic);
	
	//Visibility
	InstancedStaticMeshComponent->SetHiddenInGame(false);

	//Mobility
	//InstancedStaticMeshComponent->SetMobility(EComponentMobility::Movable);

	//Collision
	InstancedStaticMeshComponent->BodyInstance.SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	InstancedStaticMeshComponent->BodyInstance.SetCollisionProfileName("BlockAll");
	InstancedStaticMeshComponent->BodyInstance.SetObjectType(ECC_WorldStatic);
	InstancedStaticMeshComponent->BodyInstance.SetResponseToAllChannels(ECR_Block);
	InstancedStaticMeshComponent->BodyInstance.SetResponseToChannel(ECC_WorldStatic, ECR_Block);
	InstancedStaticMeshComponent->BodyInstance.SetResponseToChannel(ECC_WorldDynamic, ECR_Block);

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


	if (!StaticMeshComponent) return;

	

	//Collision
	StaticMeshComponent->BodyInstance.SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	StaticMeshComponent->BodyInstance.SetCollisionProfileName("BlockAll");
	StaticMeshComponent->BodyInstance.SetObjectType(ECC_WorldDynamic);
	StaticMeshComponent->BodyInstance.SetResponseToAllChannels(ECR_Block);
	StaticMeshComponent->BodyInstance.SetResponseToChannel(ECC_WorldStatic, ECR_Block);
	StaticMeshComponent->BodyInstance.SetResponseToChannel(ECC_WorldDynamic, ECR_Block);
}



