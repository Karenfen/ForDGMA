#include "MyPlayerState.h"
#include "Player/PlayerPawn.h"

#include "Net/UnrealNetwork.h"


AMyPlayerState::AMyPlayerState()
{
    bReplicates = true;
}

void AMyPlayerState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME_CONDITION(AMyPlayerState, Money, COND_None);
}

void AMyPlayerState::OnRep_Money()
{
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Money changed to: %d, player: %s"), Money, *GetName()));

    if (playerPawn == nullptr) {
        APawn* pawn = GetPawn();
        playerPawn = Cast<APlayerPawn>(pawn);
    }

    if (IsValid(playerPawn)) {
        playerPawn->UpdateMoney(Money);
    }
}

void AMyPlayerState::AddMoney(int addmoney)
{
    Money += addmoney;

    if (HasAuthority() && IsValid(playerPawn)) {
        playerPawn->UpdateMoney(Money);
    }
}

void AMyPlayerState::OnRep_Score()
{
    Super::OnRep_Score();

    if (IsValid(playerPawn)) {
        playerPawn->UpdateScore(Score);
    }
}

void AMyPlayerState::AddPoints(int points)
{
    Score += points;
}

void AMyPlayerState::BeginPlay()
{
    Super::BeginPlay();

    APawn* pawn = GetPawn();
    playerPawn = Cast<APlayerPawn>(pawn);
}
