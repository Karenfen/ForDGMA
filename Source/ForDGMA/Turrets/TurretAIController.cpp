#include "TurretAIController.h"
#include "MyTurretBase.h"

#include "Perception/AIPerceptionTypes.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Net/UnrealNetwork.h"



ATurretAIController::ATurretAIController()
{
	PrimaryActorTick.bCanEverTick = true;

	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception Component"));

	if (IsValid(AIPerceptionComponent)) {
		if (IsValid(SightConfig)) {
			AIPerceptionComponent->ConfigureSense(*SightConfig);
			AIPerceptionComponent->SetDominantSense(SightConfig->GetSenseImplementation());
		}
		SetPerceptionComponent(*AIPerceptionComponent);
		AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &ATurretAIController::OnTargetPerceptionUpdated);
	}
}

void ATurretAIController::BeginPlay()
{
	Super::BeginPlay();

	Turret = Cast<AMyTurretBase>(GetPawn());

	PrimaryActorTick.bCanEverTick = HasAuthority();
}

void ATurretAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (isAttack) {
		Attack();
	}
}

void ATurretAIController::OnTargetPerceptionUpdated_Implementation(AActor* actor, FAIStimulus simulus)
{
	if (simulus.Type == UAISense::GetSenseID(UAISense_Sight::StaticClass())) {
		if (simulus.WasSuccessfullySensed()) {
			isAttack = true;
			EnemyActors.AddUnique(actor);
			CurrentEnemyActor = actor;
		}
		else {
			EnemyActors.RemoveSingle(actor);

			if (EnemyActors.IsEmpty()) {
				isAttack = false;
				CurrentEnemyActor = nullptr;
			}
			else {
				CurrentEnemyActor = *(EnemyActors.begin());
			}
		}
	}
}

void ATurretAIController::Attack_Implementation()
{
	if (IsValid(Turret) && IsValid(CurrentEnemyActor)) {
		FVector targetLocation = CurrentEnemyActor->GetActorLocation();

		Turret->RotateTurretTo(targetLocation);

		if(Turret->IsLoockAtTarget(targetLocation) && Turret->IsReadyToFire()) {
			Turret->Fire();
		}
	}
}
