// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ShipPlayerController.generated.h"

class UPidRotationController;
class APathfinderPawn;
class AShipPlayerController;

DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_OneParam(FOnInputPitch, AShipPlayerController, OnInputPitchDelegate, float, Val);
DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_OneParam(FOnInputYaw, AShipPlayerController, OnInputYawDelegate, float, Val);
DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_OneParam(FOnInputRoll, AShipPlayerController, OnInputRollDelegate, float, Val);


UCLASS()
class PATHFINDING_API AShipPlayerController : public APlayerController
{
	GENERATED_BODY()
	//

public:

	AShipPlayerController();

	virtual void BeginPlay() override;

protected:

	virtual void SetupInputComponent() override;
	virtual void OnPossess(APawn* aPawn) override;
	virtual void OnUnPossess() override;

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MoveSpeed = 1000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float LateralSpeed = 1000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ElevationSpeed = 1000.f;

	UPROPERTY(BlueprintAssignable)
	FOnInputPitch OnInputPitchDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnInputYaw OnInputYawDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnInputRoll OnInputRollDelegate;

private:

	void InputUpdatedPitch(float Val);
	void InputUpdatedYaw(float Val);
	void InputUpdatedRoll(float Val);
	void InputUpdatedThrust(float Val);
	void InputUpdatedThrustLateral(float Val);
	void InputUpdatedElevation(float Val);

private:

	UPROPERTY()
	APathfinderPawn* PathfinderPawn;
};
