#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "MyGameState.generated.h"


UCLASS()
class FORDGMA_API AMyGameState : public AGameState
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Money", meta = (ToolTip = "This is the time in seconds between giving money to players"))
	float TimeToGiveMoney = 60.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Money", meta = (ToolTip = "This is the initial amount of money given out to players"))
	int StartMoneyamount = 100;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Money", meta = (ToolTip = "This is a multiplier for the amount of money given to players"))
	float MoneyMultiplier = 1.5f;

	FTimerHandle givingMoneyTimerHandle;

public:
	virtual void BeginPlay() override;
	UFUNCTION()
	void PlayerEarnedPoints(APlayerState* player, int points);

protected:
	UFUNCTION()
	void GiveMoneyToPlayers();
};
