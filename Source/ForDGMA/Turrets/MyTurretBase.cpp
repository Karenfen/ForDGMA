#include "MyTurretBase.h"
#include "../MyGameState.h"
#include "TurretAIController.h"

#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"
#include <Components/AudioComponent.h>
#include "Particles/ParticleSystemComponent.h"
#include <Kismet/KismetMathLibrary.h>
#include "Net/UnrealNetwork.h"
#include "GameFramework/PlayerState.h"
#include <Perception/AIPerceptionSystem.h>
#include "Perception/AISenseConfig_Sight.h"



AMyTurretBase::AMyTurretBase()
{
	PrimaryActorTick.bCanEverTick = false;

	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
	RootComponent = BodyMesh;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision Box"));
	CollisionBox->AttachToComponent(BodyMesh, FAttachmentTransformRules::KeepRelativeTransform);

	GunMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Gun"));
	GunMesh->AttachToComponent(BodyMesh, FAttachmentTransformRules::KeepRelativeTransform);

	LaserSpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("Laser spawn point"));
	LaserSpawnPoint->AttachToComponent(GunMesh, FAttachmentTransformRules::KeepRelativeTransform);

	LaserMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Laser"));
	LaserMesh->AttachToComponent(LaserSpawnPoint, FAttachmentTransformRules::KeepRelativeTransform);

	ShootAudio = CreateDefaultSubobject<UAudioComponent>(TEXT("Shoot audio"));
	ShootAudio->SetupAttachment(LaserSpawnPoint);
	ShootAudio->SetAutoActivate(false);

	DieAudioEffect = CreateDefaultSubobject<UAudioComponent>(TEXT("Die audio"));
	DieAudioEffect->SetupAttachment(BodyMesh);
	DieAudioEffect->SetAutoActivate(false);

	DieVisualEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Die Effect"));
	DieVisualEffect->SetupAttachment(BodyMesh);
	DieVisualEffect->SetAutoActivate(false);
}

void AMyTurretBase::SetGenericTeamId(const FGenericTeamId& newTeamID)
{
	TeamId = newTeamID;

	ATurretAIController* mycontroller = Cast<ATurretAIController>(Controller);
	if (mycontroller) {
		mycontroller->SetGenericTeamId(newTeamID);
	}
}

void AMyTurretBase::BeginPlay()
{
	Super::BeginPlay();
	
	if (IsValid(LaserMesh)) {
		LaserScale = LaserMesh->GetRelativeScale3D();
	}
}

bool AMyTurretBase::IsReadyToFire() const
{
	return ReadyToFire;
}

void AMyTurretBase::OnRep_Health()
{
	if (Health <= 0) {
		Die();
	}
}

bool AMyTurretBase::IsLoockAtTarget(const FVector& target) const
{
	if (IsValid(GunMesh)) {
		FVector gunDirection = GunMesh->GetForwardVector();
		FVector targetDirection = (target - GunMesh->GetComponentLocation()).GetSafeNormal();

		float DotProduct = FVector::DotProduct(gunDirection, targetDirection);
		float AngleInRadians = FMath::Acos(DotProduct);
		float AngleInDegrees = FMath::RadiansToDegrees(AngleInRadians);

		return (AngleInDegrees <= TargetingAngle);
	}

	return false;
}

void AMyTurretBase::Fire_Implementation()
{
	FHitResult hitResult;
	FCollisionQueryParams traceParams = FCollisionQueryParams(FName(TEXT("FireTrace")), true, this);
	traceParams.bTraceComplex = true;
	traceParams.bReturnPhysicalMaterial = false;
	traceParams.AddIgnoredActor(this);
	traceParams.AddIgnoredActor(this->GetOwner());

	FVector start = LaserSpawnPoint->GetComponentLocation();
	FVector end = start + LaserSpawnPoint->GetForwardVector() * FireRange;
	float newLaserLenght;

	if (GetWorld()->LineTraceSingleByChannel(hitResult, start, end, ECollisionChannel::ECC_Visibility, traceParams)) {
		newLaserLenght = FVector::Distance(start, hitResult.Location);
		IIDamageTaker* enemy = Cast<IIDamageTaker>(hitResult.GetActor());

		if (enemy) {
			enemy->TakeDamage(FireDamage, nullptr);
		}
	}
	else {
		newLaserLenght = FireRange;
	}

	if(IsValid(LaserMesh)) {
		LaserScale.Z = newLaserLenght;
		LaserMesh->SetRelativeScale3D(LaserScale);
		LaserMesh->SetHiddenInGame(false);
		GetWorld()->GetTimerManager().SetTimer(LaserLifeTimerHandle, this, &AMyTurretBase::LaserOff, LasetLifeTime, false);
	}

	if (ShootAudio)
		ShootAudio->Play();

	ReadyToFire = false;
	GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &AMyTurretBase::Reload, 1.0f / FireRate, false);
}

void AMyTurretBase::LaserOff()
{
	if (IsValid(LaserMesh)) {
		LaserMesh->SetHiddenInGame(true);
	}
}

void AMyTurretBase::RotateTurretTo_Implementation(const FVector& TargetPosition)
{
	FRotator targetRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), TargetPosition);
	FRotator turretRotation = GunMesh->GetComponentRotation();
	targetRotation.Roll = turretRotation.Roll;

	GunMesh->SetWorldRotation(FMath::Lerp(turretRotation, targetRotation, GunRotationInterpolationKey));

}

bool AMyTurretBase::RotateTurretTo_Validate(const FVector& TargetPosition)
{
	return IsValid(GunMesh);
}

void AMyTurretBase::Reload_Implementation()
{
	ReadyToFire = true;
}

void AMyTurretBase::Die()
{
	if (IsValid(Controller)) {
		Controller->Destroy();
	}

	if (IsValid(BodyMesh)) {
		BodyMesh->SetHiddenInGame(true);
	}

	if (IsValid(GunMesh)) {
		GunMesh->SetHiddenInGame(true);
	}

	if (IsValid(LaserMesh)) {
		LaserMesh->SetHiddenInGame(true);
	}

	if (IsValid(DieAudioEffect)) {
		DieAudioEffect->Play();
	}

	if (IsValid(DieVisualEffect)) {
		DieVisualEffect->ActivateSystem();
	}

	SetActorEnableCollision(false);
	
	GetWorld()->GetTimerManager().SetTimer(DestroyTimerHandle, this, &AMyTurretBase::Destroy_, TimeToDestroy, false);
}

void AMyTurretBase::Destroy_()
{
	Destroy();
}

void AMyTurretBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AMyTurretBase::TakeDamage_Implementation(int damage, APlayerState* instigator)
{
	Health -= damage;

	if (Health <= 0) {
		if (HasAuthority()) {
			AMyGameState* gameState = Cast<AMyGameState>(GetWorld()->GetGameState());

			if (IsValid(gameState)) {
				gameState->PlayerEarnedPoints(playerOwnerState, Points);
			}

			Die();
		}
	}
}

void AMyTurretBase::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(AMyTurretBase, Health, COND_OwnerOnly);
}

void AMyTurretBase::SetPlayerOwner_Implementation(APlayerState* playerState) 
{
	playerOwnerState = playerState;
}