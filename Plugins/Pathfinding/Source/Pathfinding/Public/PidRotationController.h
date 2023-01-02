// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PhysicsPublic.h"
#include "Components/ActorComponent.h"
#include "PidRotationController.generated.h"


class UPidController;
class UPidController3Axis;
class APlayerController;
//
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class PATHFINDING_API UPidRotationController : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPidRotationController();

private:

    /** Works by using its own fixed update and is not updated from an external script */
	UPROPERTY(CATEGORY="Pid Update", EditInstanceOnly)
    bool isStandalone;

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UPidController3Axis* PidLookDirection;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UPidController* PidStabilizeAngularVelocity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AngularDrag = 3.0f;
	/***/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool CheckInput = true;

	/** Invert x axis input */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool InvertPitch;
	
	/** Invert y axis input */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool InvertYaw;

	/** Invert z axis input */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool InvertRoll;

	/** Max degree rotation on y axis */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MaxPitch = 60.0f;
	
protected:

	UPROPERTY()
    UStaticMeshComponent* Body;

	FVector InputVelocity;

private:

	void ExecutePid(float DeltaTime);

	float GetPitchInput();

	float GetYawInput();

	float GetRollInput();

	void StabilizeBodyMoveRotation(float DeltaTime);

protected:
	
	virtual void BeginPlay() override;

	/** X axis rotation */
	virtual float GetPitchInputInternal();

	/** Y axis rotation */
	virtual float GetYawInputInternal();

	/** Z axis rotation */
	virtual float GetRollInputInternal();

public:

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void Init();

	void SetStaticMeshComponent(UStaticMeshComponent* BodyMesh);

	virtual void BindPlayerInput(APlayerController* PlayerController);

	virtual void UnbindPlayerInput(APlayerController* PlayerController);

	UFUNCTION()
	virtual void SetInputAngularVelocityPitch(float Val);
	UFUNCTION()
	virtual void SetInputAngularVelocityYaw(float Val);
	UFUNCTION()
	virtual void SetInputAngularVelocityRoll(float Val);

	/** Get pitch input without multiplier and with inverted controls. Returns -1 to 1 */
	float GetPitchRaw();

	/** Get yaw input without multiplier and with inverted controls. Returns -1 to 1 */
	float GetYawRaw();
	
	/** Get roll input without multiplier and with inverted controls. Returns -1 to 1 */
	float GetRollRaw();
};
