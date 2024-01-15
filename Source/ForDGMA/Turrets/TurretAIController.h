#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "TurretAIController.generated.h"


UCLASS()
class FORDGMA_API ATurretAIController : public AAIController
{
	GENERATED_BODY()
	
protected:
	// perceptions
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI Perception")
	class UAIPerceptionComponent* AIPerceptionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI Perception")
	class UAISenseConfig_Sight* SightConfig;

	class AMyTurretBase* Turret{ nullptr };
	AActor* CurrentEnemyActor{ nullptr };
	TArray<AActor*> EnemyActors;
	bool isAttack = false;

public:
	ATurretAIController();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	UFUNCTION(Server, Reliable)
	void OnTargetPerceptionUpdated(AActor* actor, struct FAIStimulus simulus);
	UFUNCTION(Server, Reliable)
	void Attack();
};
