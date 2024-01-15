#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "../IDamageTaker.h"
#include "MyTurretBase.generated.h"

UCLASS()
class FORDGMA_API AMyTurretBase : public APawn, public IIDamageTaker
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UStaticMeshComponent* BodyMesh;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UStaticMeshComponent* GunMesh;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UStaticMeshComponent* LaserMesh;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UArrowComponent* LaserSpawnPoint;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Effects")
	class UAudioComponent* ShootAudio;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Effects")
	class UAudioComponent* DieAudioEffect;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Effects")
	class UParticleSystemComponent* DieVisualEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Icon")
	TScriptInterface<class ISlateTextureAtlasInterface> Icon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Aim")
	float GunRotationInterpolationKey = 0.5f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Aim")
	float TargetingAngle = 5.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire params")
	float FireRate = 1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire params")
	float FireDamage = 1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire params")
	float FireRange = 1000.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire params")
	float LasetLifeTime = 1.0f;

	UPROPERTY(ReplicatedUsing = OnRep_Health, EditDefaultsOnly, BlueprintReadWrite, Category = "Basic")
	int Health = 100;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Basic")
	int Coast = 1000;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Basic")
	int Points = 100;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Basic")
	float TimeToDestroy = 2.0f;

	bool ReadyToFire = true;
	FVector LaserScale;
	FTimerHandle ReloadTimerHandle;
	FTimerHandle LaserLifeTimerHandle;
	FTimerHandle DestroyTimerHandle;
	UPROPERTY(Replicated)
	class APlayerState* playerOwnerState{ nullptr };

public:
	AMyTurretBase();
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;
	FORCEINLINE int GetHealth() const { return Health; };
	FORCEINLINE int GetCoast() const { return Coast; };
	UFUNCTION(Server, Reliable)
	void TakeDamage(int damage, class APlayerState* playerInstigator) override;
	UFUNCTION(Server, Unreliable, WithValidation)
	void RotateTurretTo(const FVector& target);
	UFUNCTION(Server, Reliable)
	void Fire();
	bool IsLoockAtTarget(const FVector& target) const;
	bool IsReadyToFire() const;
	UFUNCTION()
	void OnRep_Health();
	UFUNCTION(Server, Reliable)
	void SetPlayerOwner(class APlayerState* player);

protected:
	virtual void BeginPlay() override;
	void LaserOff();
	UFUNCTION(Server, Reliable)
	void Reload();
	void Die();
	void Destroy_();
};
