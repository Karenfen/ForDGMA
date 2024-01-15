#include "BuildingComponent.h"
#include "../Turrets/MyTurretBase.h"

UBuildingComponent::UBuildingComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

AMyTurretBase* UBuildingComponent::BuildTurret(const FVector& location, const FRotator& rotation)
{
	if (!TurretClass.IsEmpty()) {
		AMyTurretBase* newTurret = GetWorld()->SpawnActor<AMyTurretBase>(TurretClass.Pop(), location, rotation);

		if (newTurret) {
			return newTurret;
		}
	}

	return nullptr;
}


void UBuildingComponent::BeginPlay()
{
	Super::BeginPlay();

	
}