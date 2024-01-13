#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "MyPlayerState.generated.h"


UCLASS()
class FORDGMA_API AMyPlayerState : public APlayerState
{
	GENERATED_BODY()

protected:
    UPROPERTY(ReplicatedUsing = OnRep_Money, EditDefaultsOnly, BlueprintReadOnly, Category = "Money")
    int Money = 0;

    class APlayerPawn* playerPawn{ nullptr };

public:
    AMyPlayerState();

    UFUNCTION()
    void OnRep_Money();

    UFUNCTION()
    int GetMoney() const { return Money; };

	UFUNCTION()
	void AddMoney(int addmoney);

protected:
    virtual void BeginPlay() override;
};
