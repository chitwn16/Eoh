// Fill out your copyright notice in the Description page of Project Settings.


#include "PathfinderNavActorComponent.h"
#include "Pathfinding/Public/PidController.h"
#include "Pathfinding/Public/PidController3Axis.h"
#include "Pathfinding/Public/PathfindingFunctionLibrary.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"

// Sets default values for this component's properties
UPathfinderNavActorComponent::UPathfinderNavActorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickGroup = TG_PrePhysics;
	//PrimaryComponentTick.TickGroup = TG_DuringPhysics;
	PidLookDirection = CreateDefaultSubobject<UPidController3Axis>(TEXT("PidLookDirection"));
	PidStabilizeAngularVelocity = CreateDefaultSubobject<UPidController3Axis>(TEXT("PidStabilizeAngularVelocity"));
	PidStabilizeRoll = CreateDefaultSubobject<UPidController3Axis>(TEXT("PidStabilizeRoll"));
}


// Called when the game starts
void UPathfinderNavActorComponent::BeginPlay()
{
	Super::BeginPlay();
	CachePathfinder();
	Body = Cast<UStaticMeshComponent>(GetOwner()->GetComponentByClass(UStaticMeshComponent::StaticClass()));
	// TODO: try with this (only from physics thread)
	OnCalculateCustomPhysics.BindUObject(this, &UPathfinderNavActorComponent::PhysicsTick);
	//Body->BodyInstance.AddCustomPhysics(OnCalculateCustomPhysics);
}


// Called every frame
void UPathfinderNavActorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	UpdateBehaviour(DeltaTime);
}

void UPathfinderNavActorComponent::StabilizeBodyAngularVelocity(float DeltaTime)
{
	FVector Velocity = Body->GetPhysicsAngularVelocityInRadians();
	FVector Torque = PidStabilizeAngularVelocity->Cycle(Velocity * -1.f, DeltaTime);
	Body->AddTorqueInRadians(Torque, FName(EName::None), true);
}

void UPathfinderNavActorComponent::LookAtDirection(float DeltaTime, FVector Direction)
{
	FVector Cross = FVector::CrossProduct(Body->GetForwardVector(), Direction);
	FVector Torque = PidLookDirection->Cycle(Cross, DeltaTime);
	Body->AddTorqueInRadians(Torque, FName(EName::None), true);
}

void UPathfinderNavActorComponent::StabilizeOrientationAxis(float DeltaTime)
{
	// TODO: calculate angular velocity (torque) required to stabilize roll
	FRotator TargetRotator = Body->GetRelativeRotation();
	TargetRotator.SetComponentForAxis(EAxis::X, 0.0);
	FVector Orientation = PidStabilizeRoll->Cycle(TargetRotator.Vector(), DeltaTime);
	Body->SetRelativeRotation(Orientation.Rotation(), true);
}

// https://docs.unrealengine.com/5.0/en-US/API/Runtime/Engine/FCalculateCustomPhysics/
void UPathfinderNavActorComponent::PhysicsTick(float DeltaTime, FBodyInstance* BodyInstance)
{
}

void UPathfinderNavActorComponent::UpdateBehaviour(float DeltaTime)
{
	if (GetPathfinder())
	{
		StabilizeBodyAngularVelocity(DeltaTime);
		StabilizeOrientationAxis(DeltaTime);
		if (!HasReachedDestination && GetPathfinder()->GetHasFoundPath())
		{
			Body->SetPhysicsLinearVelocity(FVector::Zero());
			int32 PathNum = GetPathfinder()->GetPathNum();
			FVector Location = GetOwner()->GetActorLocation();
			if (CurrentPathPointIndex < 0)
			{
				CurrentPathPointIndex = PathNum;
				for (int32 Index = 1; Index < PathNum; Index++)
				{
					FVector TargetLocation = GetPathfinder()->PathVector[Index];
					float Distance = FVector::Dist(Location, TargetLocation);
					if (Distance > PathPointReachedDistance)
					{
						CurrentPathPointIndex = Index;
						break;
					}
				}
			}
			if (CurrentPathPointIndex < PathNum)
			{
				FVector TargetLocation = GetPathfinder()->PathVector[CurrentPathPointIndex];
				float Distance = FVector::Dist(Location, TargetLocation);
				if (Distance <= PathPointReachedDistance)
				{
					CurrentPathPointIndex++;
				}
				else
				{
					FVector Direction = TargetLocation - Location;
					Direction.Normalize();
					Body->AddImpulse(Body->GetForwardVector() * MoveSpeed * DeltaTime, FName(EName::None), true);
					LookAtDirection(DeltaTime, Direction);
				}
			}
			if (!HasReachedDestination)
			{
				HasReachedDestination = CurrentPathPointIndex == PathNum;
				if (HasReachedDestination)
				{
					// stop? if not
				}
			}
		}
	}
}

UPathfinderComponent* UPathfinderNavActorComponent::GetPathfinder()
{
	if (!Pathfinder)
	{
		CachePathfinder();
	}
	return Pathfinder;
}

void UPathfinderNavActorComponent::CachePathfinder()
{
	Pathfinder = Cast<UPathfinderComponent>(PathfinderReference.GetComponent(GetOwner()));
}

void UPathfinderNavActorComponent::SetGrid(UNodeGridActorComponent* NodeGridActor)
{
	NodeGrid = NodeGridActor;
}

void UPathfinderNavActorComponent::FindPath(const FVector From, const FVector To)
{
	if (!GetPathfinder() || !NodeGrid)
	{
		return;
	}
	GetPathfinder()->FindPath(NodeGrid->GetGrid(), From, To);
	CurrentPathPointIndex = -1;
	HasReachedDestination = false;
}

