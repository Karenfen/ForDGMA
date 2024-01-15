#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BuildingComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FORDGMA_API UBuildingComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turrets")
	TArray<TSubclassOf<class AMyTurretBase>> TurretClass;

public:	
	UBuildingComponent();

	AMyTurretBase* BuildTurret(const FVector& location, const FRotator& rotation);

protected:
	virtual void BeginPlay() override;	
};
