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
		FVector playerPosition = PlayerPawn->GetActorLocation();

		DeprojectMousePositionToWorld(mousePosition, mouseDirection);

		float distance = FVector::Distance(mousePosition, playerPosition);
		distance /= UKismetMathLibrary::Cos(acosf(FVector::DotProduct(mouseDirection, playerPosition.DownVector)));

		mousePosition = mousePosition + (mouseDirection * distance);

		PlayerPawn->SetCursorLocation(mousePosition);
	}


}
