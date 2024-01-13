#include "MyGameState.h"
#include "MyPlayerState.h"

#include "TimerManager.h"


void AMyGameState::BeginPlay()
{
	GetWorld()->GetTimerManager().SetTimer(givingMoneyTimerHandle, this, &AMyGameState::GiveMoneyToPlayers, TimeToGiveMoney, true);
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
