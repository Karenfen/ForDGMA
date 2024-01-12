#include "MyPlayerController.h"

#include"PlayerPawn.h"

void AMyPlayerController::BeginPlay() 
{
	Super::BeginPlay();

	PlayerPawn = Cast<APlayerPawn>(GetPawn());
}


void AMyPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAxis("MoveForward", this, &AMyPlayerController::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &AMyPlayerController::MoveRight);
}

void AMyPlayerController::MoveForward(float AxisValue)
{
	if (IsValid(PlayerPawn)) {
		PlayerPawn->_targetForwardAxisValue = AxisValue;
	}
}

void AMyPlayerController::MoveRight(float AxisValue)
{
	if (IsValid(PlayerPawn)) {
		PlayerPawn->_targetRightdAxisValue = AxisValue;
	}
}
