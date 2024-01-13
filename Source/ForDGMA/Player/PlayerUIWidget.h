#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerUIWidget.generated.h"


UCLASS()
class FORDGMA_API UPlayerUIWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* TextBlock_Money;

public:
	void UpdateMoney(int newMoney);
};
