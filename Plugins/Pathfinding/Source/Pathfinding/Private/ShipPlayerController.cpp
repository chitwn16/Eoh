// Fill out your copyright notice in the Description page of Project Settings.


#include "ShipPlayerController.h"
#include "Pathfinding/Public/PidRotationController.h"
#include "Pathfinding/Public/PathfinderPawn.h"
#include "Pathfinding/Public/PathfinderComponent.h"

AShipPlayerController::AShipPlayerController()
{
}

void AShipPlayerController::BeginPlay()
{
	Super::BeginPlay();
}

void AShipPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent.Get()->BindAxis(TEXT("MovePitch"), this, &AShipPlayerController::InputUpdatedPitch);
	InputComponent.Get()->BindAxis(TEXT("MoveYaw"), this, &AShipPlayerController::InputUpdatedYaw);
	InputComponent.Get()->BindAxis(TEXT("MoveRoll"), this, &AShipPlayerController::InputUpdatedRoll);
	InputComponent.Get()->BindAxis(TEXT("Thrust"), this, &AShipPlayerController::InputUpdatedThrust);
	InputComponent.Get()->BindAxis(TEXT("ThrustLateral"), this, &AShipPlayerController::InputUpdatedThrustLateral);
	InputComponent.Get()->BindAxis(TEXT("Elevation"), this, &AShipPlayerController::InputUpdatedElevation);
}

void AShipPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);
	PathfinderPawn = Cast<APathfinderPawn>(aPawn);
	if (PathfinderPawn)
	{
		if (PathfinderPawn->GetPathfinder())
		{
			// stop from auto updating movement
			PathfinderPawn->GetPathfinder()->SetComponentTickEnabled(false);
		}
		if (PathfinderPawn->GetRotationController())
		{
			PathfinderPawn->GetRotationController()->SetComponentTickEnabled(true);
			PathfinderPawn->GetRotationController()->BindPlayerInput(this);
		}
	}
}

void AShipPlayerController::OnUnPossess()
{
	Super::OnUnPossess();
	if (PathfinderPawn)
	{
		if (PathfinderPawn->GetRotationController())
		{
			PathfinderPawn->GetRotationController()->SetComponentTickEnabled(false);
			PathfinderPawn->GetRotationController()->UnbindPlayerInput(this);
		}
	}
	PathfinderPawn = nullptr;
}

void AShipPlayerController::InputUpdatedPitch(float Val)
{
	OnInputPitchDelegate.Broadcast(Val);
}

void AShipPlayerController::InputUpdatedYaw(float Val)
{
	OnInputYawDelegate.Broadcast(Val);
}

void AShipPlayerController::InputUpdatedRoll(float Val)
{
	OnInputRollDelegate.Broadcast(Val);
}

void AShipPlayerController::InputUpdatedThrust(float Val)
{
	if (PathfinderPawn && PathfinderPawn->GetBody())
	{
		PathfinderPawn->GetBody()->AddForce(PathfinderPawn->GetActorForwardVector() * MoveSpeed * Val, FName(EName::None), true);
	}
}

void AShipPlayerController::InputUpdatedThrustLateral(float Val)
{
	if (PathfinderPawn && PathfinderPawn->GetBody())
	{
		PathfinderPawn->GetBody()->AddForce(PathfinderPawn->GetActorRightVector() * LateralSpeed * Val, FName(EName::None), true);
	}
}

void AShipPlayerController::InputUpdatedElevation(float Val)
{
	if (PathfinderPawn && PathfinderPawn->GetBody())
	{
		PathfinderPawn->GetBody()->AddForce(PathfinderPawn->GetActorUpVector() * ElevationSpeed * Val, FName(EName::None), true);
	}
}
