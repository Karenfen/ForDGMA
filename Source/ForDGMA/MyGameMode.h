#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "GenericTeamAgentInterface.h"
#include "MyGameMode.generated.h"


UCLASS()
class FORDGMA_API AMyGameMode : public AGameMode
{
	GENERATED_BODY()

protected:
	FTimerHandle givingMoneyTimerHandle;
	uint8 TeamIDuint8 = 1;

public:
	void PostLogin(APlayerController* NewPlayer) override;

	FGenericTeamId GenerateTeamID();

//
//	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Players")
//	int PlayerCount = 1;
//
//public:
//	// Переопределение функции ReadyToStartMatch
//	virtual bool ReadyToStartMatch_Implementation() override;
	
};
