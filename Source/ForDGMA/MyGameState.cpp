#include "MyGameState.h"
#include "MyPlayerState.h"
#include "player/MyPlayerController.h"

#include "TimerManager.h"


void AMyGameState::BeginPlay()
{
	GetWorld()->GetTimerManager().SetTimer(givingMoneyTimerHandle, this, &AMyGameState::GiveMoneyToPlayers, TimeToGiveMoney, true);
}

void AMyGameState::PlayerEarnedPoints(APlayerState* player, int points)
{
	if (PlayerArray.Contains(player)) {
		AMyPlayerState* playerState = Cast<AMyPlayerState>(player);

		if (IsValid(playerState)) {
			playerState->AddPoints(points);
		}
	}
}

void AMyGameState::GiveMoneyToPlayers()
{
	for (const auto& player : PlayerArray) {
		AMyPlayerState* playerState = Cast<AMyPlayerState>(player);

		if (IsValid(playerState)) {
			playerState->AddMoney(StartMoneyamount);
		}
	}

	StartMoneyamount *= MoneyMultiplier;
}