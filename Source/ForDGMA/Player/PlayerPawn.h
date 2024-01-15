#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PlayerPawn.generated.h"


UCLASS()
class FORDGMA_API APlayerPawn : public APawn
{
	GENERATED_BODY()

public:

	float _targetForwardAxisValue = 0.0f;
	float _targetRightdAxisValue = 0.0f;

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UCameraComponent* Camera;

	// Cursor collider compponent
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class USphereComponent* CursorCollider;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UBuildingComponent* BuildComponent;

	// Cursor widget compponent
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<class UPlayerUIWidget> UIWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement|Speed")
	float MoveSpeed = 400.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Cursor")
	float CursorCollisionRadius = 10.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Cursor")
	float MaxCursorDistance = 10000.0f;

	class UPlayerUIWidget* UIWidget{ nullptr };
	bool IsBuildingAvalible = true;

public:
	APlayerPawn();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void SetCursorLocation(const FVector& mousePosition, const FVector& mouseDirection);

	UFUNCTION()
	void UpdateMoney(int money);

	UFUNCTION()
	void UpdateScore(int score);

	void FirstAction();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void Move(float DeltaTime);

	UFUNCTION(Server, Reliable, WithValidation)
	void SpawnTurret();

	UFUNCTION(Server, Unreliable)
	void ServerSetLocation(FVector NewLocation);
	
	UFUNCTION(Server, Reliable)
	void SetBuildingAvalible(bool isAvalible);

	UFUNCTION(Server, Unreliable)
	void SetCursorLocationOnServer(const FVector& newPosition);

};