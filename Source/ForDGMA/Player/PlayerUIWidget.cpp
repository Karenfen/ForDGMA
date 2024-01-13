#include "PlayerUIWidget.h"

#include "Components/TextBlock.h"
#include "Kismet/KismetTextLibrary.h"



void UPlayerUIWidget::UpdateMoney(int newMoney)
{
	if (IsValid(TextBlock_Money)) {
		TextBlock_Money->SetText(UKismetTextLibrary::Conv_IntToText(newMoney));
	}
}