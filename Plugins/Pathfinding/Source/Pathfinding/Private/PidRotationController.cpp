// Fill out your copyright notice in the Description page of Project Settings.

#include "PidRotationController.h"
#include "Components/StaticMeshComponent.h"
#include "Pathfinding/Public/PidController.h"
#include "Pathfinding/Public/PidController3Axis.h"
#include "Pathfinding/Public/ShipPlayerController.h"

// Sets default values for this component's properties
UPidRotationController::UPidRotationController()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.TickGroup = TG_PrePhysics;
	PidLookDirection = CreateDefaultSubobject<UPidController3Axis>(TEXT("PidLookDirection"));
	PidStabilizeAngularVelocity = CreateDefaultSubobject<UPidController>(TEXT("PidStabilizeAngularVelocity"));
}

void UPidRotationController::BeginPlay()
{
	Super::BeginPlay();
}

void UPidRotationController::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Body->BodyInstance.AngularDamping = AngularDrag;
	ExecutePid(DeltaTime);
	StabilizeBodyMoveRotation(DeltaTime);
}

void UPidRotationController::Init()
{
}

void UPidRotationController::SetStaticMeshComponent(UStaticMeshComponent* BodyMesh)
{
	Body = BodyMesh;
}

void UPidRotationController::BindPlayerInput(APlayerController* PlayerController)
{
	AShipPlayerController* Controller = Cast<AShipPlayerController>(PlayerController);
	if (Controller)
	{
		Controller->OnInputPitchDelegate.AddDynamic(this, &UPidRotationController::SetInputAngularVelocityPitch);
		Controller->OnInputYawDelegate.AddDynamic(this, &UPidRotationController::SetInputAngularVelocityYaw);
		Controller->OnInputRollDelegate.AddDynamic(this, &UPidRotationController::SetInputAngularVelocityRoll);
	}
}

void UPidRotationController::UnbindPlayerInput(APlayerController* PlayerController)
{
	AShipPlayerController* Controller = Cast<AShipPlayerController>(PlayerController);
	if (Controller)
	{
		Controller->OnInputPitchDelegate.RemoveDynamic(this, &UPidRotationController::SetInputAngularVelocityPitch);
		Controller->OnInputYawDelegate.RemoveDynamic(this, &UPidRotationController::SetInputAngularVelocityYaw);
		Controller->OnInputRollDelegate.RemoveDynamic(this, &UPidRotationController::SetInputAngularVelocityRoll);
	}
}

void UPidRotationController::SetInputAngularVelocityPitch(float Val)
{
	InputVelocity.Y = Val;
}

void UPidRotationController::SetInputAngularVelocityYaw(float Val)
{
	InputVelocity.Z = Val;
}

void UPidRotationController::SetInputAngularVelocityRoll(float Val)
{
	InputVelocity.X = Val;
}

void UPidRotationController::ExecutePid(float DeltaTime)
{
	PidLookDirection->SetBounds();
	//determine target rates of rotation based on user input as a percentage of the maximum angular velocity
	FVector TargetVelocity = FVector(
        GetPitchInput(),
        GetYawInput(),
        GetRollInput()
    );

	InputVelocity.X *= (InvertRoll ? -1.0f : 1.0f);
	InputVelocity.Y *= (InvertPitch ? -1.0f : 1.0f);
	InputVelocity.Z *= (InvertYaw ? -1.0f : 1.0f);
	
	//take the rigidbody.angularVelocity and convert it to local space; we need this for comparison to target rotation velocities
	const FVector CurrentVelocity = Body->GetComponentTransform().InverseTransformVector(Body->GetPhysicsAngularVelocityInRadians());
	
	// run the controller
	PidLookDirection->Cycle(CurrentVelocity, InputVelocity, DeltaTime);
	Body->AddTorqueInRadians(Body->GetComponentTransform().TransformVector(InputVelocity * PidLookDirection->Kp), FName(EName::None), true);
	// Unity: body.AddRelativeTorque(pid.output * Time.fixedDeltaTime, ForceMode.Acceleration);
	InputVelocity = FVector::ZeroVector;
}

void UPidRotationController::StabilizeBodyMoveRotation(float DeltaTime)
{
	return;
	FRotator Rotation = Body->GetComponentTransform().Rotator();
	FRotator NewRotation = FRotator(
		Rotation.Pitch,
		Rotation.Yaw,
		PidStabilizeAngularVelocity->Update(0.0f, Rotation.Roll, DeltaTime)
	);

	Body->SetAllPhysicsRotation(NewRotation);
	// Unity: body.MoveRotation(rotation);
}

float UPidRotationController::GetPitchInput()
{
	if (!CheckInput) return 0.0f;

	float PitchAngle = Body->GetComponentTransform().Rotator().Pitch;
	if (PitchAngle > 180.0f) PitchAngle -= 360.0f;

	// Don't let pitch above target
	float PitchRotation = GetPitchRaw();

	if (PitchAngle > MaxPitch && PitchRotation > 0.0f)
	{
		PitchRotation = 0.0f;
	}

	else if (PitchAngle < -MaxPitch && PitchRotation < 0.0f)
	{
		PitchRotation = 0.0f;
	}

	return PitchRotation;
}

float UPidRotationController::GetYawInput()
{
	if (!CheckInput) return 0.0f;
	return GetYawRaw();
}

float UPidRotationController::GetRollInput()
{
	if (!CheckInput) return 0.0f;
	return GetRollRaw();
}

float UPidRotationController::GetPitchRaw()
{
	return -GetPitchInputInternal() * (InvertPitch ? -1.0f : 1.0f);
}

float UPidRotationController::GetYawRaw()
{
	return GetYawInputInternal() * (InvertYaw ? -1.0f : 1.0f);
}

float UPidRotationController::GetRollRaw()
{
	return GetRollInputInternal() * (InvertRoll ? -1.0f : 1.0f);
}

float UPidRotationController::GetPitchInputInternal()
{
	return 0.0f;
}

float UPidRotationController::GetYawInputInternal()
{
	return 0.0f;
}

float UPidRotationController::GetRollInputInternal()
{
	return 0.0f;
}

