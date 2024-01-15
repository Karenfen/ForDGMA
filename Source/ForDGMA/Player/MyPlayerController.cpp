#include "MyPlayerController.h"

#include"PlayerPawn.h"
#include <Kismet/KismetMathLibrary.h>


void AMyPlayerController::BeginPlay() 
{
	Super::BeginPlay();

	PlayerPawn = Cast<APlayerPawn>(GetPawn());
}

void AMyPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MousePositionUpdate();
}


void AMyPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAxis("MoveForward", this, &AMyPlayerController::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &AMyPlayerController::MoveRight);
	InputComponent->BindAction("ClickLeft", EInputEvent::IE_Pressed, this, &AMyPlayerController::ClickLeft);
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

void AMyPlayerController::MousePositionUpdate()
{
	if(IsValid(PlayerPawn)) {
		FVector mouseDirection;
		FVector mousePosition;

		DeprojectMousePositionToWorld(mousePosition, mouseDirection);

		PlayerPawn->SetCursorLocation(mousePosition, mouseDirection);
	}


}

void AMyPlayerController::ClickLeft()
{
	if (IsValid(PlayerPawn)) {
		PlayerPawn->FirstAction();
	}
}