#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IDamageTaker.generated.h"

UINTERFACE(MinimalAPI)
class UIDamageTaker : public UInterface
{
	GENERATED_BODY()
};


class FORDGMA_API IIDamageTaker
{
	GENERATED_BODY()

public:
	virtual void TakeDamage(int damage, class APlayerState* instigator) = 0;
};
