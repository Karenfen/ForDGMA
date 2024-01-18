#include "MyGameMode.h"
#include "Player/MyPlayerController.h"

//bool AMyGameMode::ReadyToStartMatch_Implementation()
//{
//    if (PlayerCount == NumPlayers) {
//        return Super::ReadyToStartMatch_Implementation();
//    }
//    else {
//        return false;
//    }
//}

FGenericTeamId AMyGameMode::GenerateTeamID()
{
	return FGenericTeamId(TeamIDuint8++);
}

void AMyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	AMyPlayerController* player = Cast<AMyPlayerController>(NewPlayer);

	if (player) {
		player->SetGenericTeamId(GenerateTeamID());
	}
}
