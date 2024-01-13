#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class FORDGMA_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:
	UPROPERTY()
	class APlayerPawn* PlayerPawn{ nullptr };
	
public:
	virtual void SetupInputComponent() override;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);
	void MousePositionUpdate();
};
