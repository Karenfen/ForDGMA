#include "PlayerPawn.h"
#include "PlayerUIWidget.h"
#include "../MyPlayerState.h"

#include <GameFramework/SpringArmComponent.h>
#include <Camera/CameraComponent.h>
#include "Net/UnrealNetwork.h"
#include <Kismet/KismetMathLibrary.h>
#include "Components/SphereComponent.h"



// Sets default values
APlayerPawn::APlayerPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring arm"));
	SpringArm->SetupAttachment(Mesh);
	SpringArm->bDoCollisionTest = false;
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritYaw = false;
	SpringArm->bInheritRoll = false;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	CursorCollider = CreateDefaultSubobject<USphereComponent>(TEXT("Cursor Collision"));
	CursorCollider->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void APlayerPawn::BeginPlay()
{
	Super::BeginPlay();
	
	if (IsValid(CursorCollider)) {
		CursorCollider->InitSphereRadius(CursorCollisionRadius);
	}

	if (UIWidgetClass)
	{
		UIWidget = CreateWidget<UPlayerUIWidget>(GetWorld(), UIWidgetClass);

		if (IsValid(UIWidget)) {
			UIWidget->AddToViewport();
			UIWidget->UpdateMoney(0);
		}
	}
}

// Called every frame
void APlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Move(DeltaTime);
	CheckGround();
}

// Called to bind functionality to input
void APlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void APlayerPawn::SetCursorLocation(const FVector& mousePosition)
{
	if (IsValid(CursorCollider)) {
		CursorCollider->SetWorldLocation(mousePosition);
	}
}

void APlayerPawn::Move(float DeltaTime)
{
	FVector currentLocation = GetActorLocation();
	FVector forwardVector = GetActorForwardVector();
	FVector rightVector = GetActorRightVector();
	FVector movePosition = currentLocation + ((forwardVector * _targetForwardAxisValue + rightVector * _targetRightdAxisValue) * MoveSpeed * DeltaTime);
	
	if (HasAuthority()) {
		SetActorLocation(movePosition, true);
	}
	else {
		ServerSetLocation(movePosition); 
	}
}

void APlayerPawn::CheckGround()
{
	if (IsValid(CursorCollider)) {
		FHitResult hitResult;
		FCollisionQueryParams traceParams = FCollisionQueryParams(FName(TEXT("FireTrace")), true, this);
		traceParams.bTraceComplex = true;
		traceParams.bReturnPhysicalMaterial = true;
		traceParams.AddIgnoredActor(this);

		FVector start = CursorCollider->GetComponentLocation();
		FVector end = start - (CursorCollider->GetUpVector() * 500.0f);

		if (GetWorld()->LineTraceSingleByChannel(hitResult, start, end, ECollisionChannel::ECC_Visibility, traceParams))
		{
			UPhysicalMaterial* HitPhysMaterial = hitResult.PhysMaterial.Get();

			if (HitPhysMaterial && HitPhysMaterial->GetName() == "ConstructibleGroundMaterial") {
				//GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Green, TEXT("ConstructibleGroundMaterial"));
			}
			else {
				//GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Red, TEXT("Nothing"));
			}
		}
	}
}

void APlayerPawn::UpdateMoney(int money)
{
	if (IsValid(UIWidget)) {
		UIWidget->UpdateMoney(money);
	}
}

void APlayerPawn::UpdateScore(int score)
{
	if (IsValid(UIWidget)) {
		//UIWidget->UpdateMoney(money);
	}
}

void APlayerPawn::ServerSetLocation_Implementation(FVector NewLocation)
{
	SetActorLocation(NewLocation, true);
}