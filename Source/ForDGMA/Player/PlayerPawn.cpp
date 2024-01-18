#include "PlayerPawn.h"
#include "PlayerUIWidget.h"
#include "../Components/BuildingComponent.h"
#include "../MyGameMode.h"
#include "../MyPlayerState.h"

#include <GameFramework/SpringArmComponent.h>
#include <Camera/CameraComponent.h>
#include "Net/UnrealNetwork.h"
#include <Kismet/KismetMathLibrary.h>
#include "Components/SphereComponent.h"
#include <ForDGMA/Turrets/MyTurretBase.h>
#include <ForDGMA/MyGameState.h>



APlayerPawn::APlayerPawn()
{
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

	BuildComponent = CreateDefaultSubobject<UBuildingComponent>(TEXT("Building component"));
}

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

	InitTeamIDOnServer();
}

// Called every frame
void APlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Move(DeltaTime);
}

// Called to bind functionality to input
void APlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void APlayerPawn::SetCursorLocation(const FVector& mousePosition, const FVector& mouseDirection)
{
	if (IsValid(CursorCollider)) {
		FVector newCursorLocation = CursorCollider->GetComponentLocation();
		FHitResult hitResult;
		FCollisionQueryParams traceParams = FCollisionQueryParams(FName(TEXT("FireTrace")), true, this);
		traceParams.bTraceComplex = true;
		traceParams.bReturnPhysicalMaterial = true;
		traceParams.AddIgnoredActor(this);

		FVector end = mousePosition + (mouseDirection * MaxCursorDistance);

		if (GetWorld()->LineTraceSingleByChannel(hitResult, mousePosition, end, ECollisionChannel::ECC_Visibility, traceParams))
		{
			UPhysicalMaterial* HitPhysMaterial = hitResult.PhysMaterial.Get();

			if (HitPhysMaterial && HitPhysMaterial->GetName() == "ConstructibleGroundMaterial") {
				SetBuildingAvalible(true);
				IsBuildingAvalible = true;
			}
			else {
				SetBuildingAvalible(false);
				IsBuildingAvalible = false;
			}

			newCursorLocation = hitResult.ImpactPoint;
		}

		CursorCollider->SetWorldLocation(newCursorLocation);
		SetCursorLocationOnServer(newCursorLocation);
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

void APlayerPawn::InitTeamIDOnServer_Implementation()
{
	if (TeamId == FGenericTeamId::NoTeam) {
		AMyGameMode* myGameMode = Cast<AMyGameMode>(GetWorld()->GetAuthGameMode());

		if (myGameMode) {
			TeamId = myGameMode->GenerateTeamID();
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("New teamID: %d, player: %s"), TeamId, *GetName()));
		}
	}
}

void APlayerPawn::SetCursorLocationOnServer_Implementation(const FVector& newPosition)
{
	if (IsValid(CursorCollider)) {
		CursorCollider->SetWorldLocation(newPosition);
	}
}

void APlayerPawn::SetBuildingAvalible_Implementation(bool isAvalible)
{
	IsBuildingAvalible = isAvalible;
}

void APlayerPawn::SpawnTurret_Implementation()
{
	AMyTurretBase* newTurret = BuildComponent->BuildTurret(CursorCollider->GetComponentLocation(), CursorCollider->GetComponentRotation());

	if (IsValid(newTurret)) {
		newTurret->SetPlayerOwner(GetPlayerState<APlayerState>());
		newTurret->SetGenericTeamId(TeamId);
	}
}

bool APlayerPawn::SpawnTurret_Validate()
{
	return (IsValid(BuildComponent) && IsValid(CursorCollider));
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

void APlayerPawn::FirstAction()
{
	if (IsBuildingAvalible) {
		SpawnTurret();
	}
}

void APlayerPawn::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(APlayerPawn, TeamId, COND_OwnerOnly);

}

void APlayerPawn::ServerSetLocation_Implementation(FVector NewLocation)
{
	SetActorLocation(NewLocation, true);
}