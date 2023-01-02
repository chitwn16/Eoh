// Fill out your copyright notice in the Description page of Project Settings.


#include "PathfinderPawn.h"
#include "Pathfinding/Public/PathfinderNavActorComponent.h"
#include "Pathfinding/Public/NodeGridActorComponent.h"
#include "Pathfinding/Public/PathfindingFunctionLibrary.h"
#include "Pathfinding/Public/PidRotationController.h"

// Sets default values
APathfinderPawn::APathfinderPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Body->SetupAttachment(RootComponent);
	NavActor = CreateDefaultSubobject<UPathfinderNavActorComponent>(TEXT("Pathfinder Nav Actor"));
	PidRotationController = CreateDefaultSubobject<UPidRotationController>(TEXT("Pid Rotation Controller"));
	AddOwnedComponent(NavActor);
	AddOwnedComponent(PidRotationController);
}

// Called when the game starts or when spawned
void APathfinderPawn::BeginPlay()
{
	Super::BeginPlay();
	if (WorldGrid)
	{
		NodeGrid = Cast<UNodeGridActorComponent>(WorldGrid->GetComponentByClass(UNodeGridActorComponent::StaticClass()));
	}
	PidRotationController->SetStaticMeshComponent(Body);
	CachePathfinder();
	NavActor->SetGrid(NodeGrid);
}

// Called every frame
void APathfinderPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	/*
	if (PathUpdateTime > 0.f)
	{
		PathUpdateTimeLast -= DeltaTime;
		if (PathUpdateTimeLast <= 0.f)
		{
			PathUpdateTimeLast = PathUpdateTime;
			FindPathToTargetWorldActor();
		}
	}*/
	if (DebugDrawLastPath)
	{
		if (Pathfinder && Pathfinder->GetHasFoundPath())
		{
			UPathfindingFunctionLibrary::DrawDebugPath(GetWorld(), Pathfinder->Path, DebugPathColor, false, -1.f, 0U, DebugPathLineThickness);
		}
	}
	if (NodeGrid)
	{
		UPathfindingFunctionLibrary::DrawCurrentNodePosition(GetWorld(), NodeGrid->GetGrid(), (NodeGrid->NodeSize) + 10.f, GetActorLocation(), FColor::Green);
	}
}

// Called to bind functionality to input
void APathfinderPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void APathfinderPawn::CachePathfinder()
{
	Pathfinder = Cast<UPathfinderComponent>(PathfinderReference.GetComponent(this));
}

void APathfinderPawn::FindPathToTargetWorldActor()
{
	if (WorldActorTargetPosition)
	{
		FindPath(GetTransform(), WorldActorTargetPosition->GetTransform());
	}
}

void APathfinderPawn::FindPathFromSelf(const FTransform To)
{
	FindPath(GetTransform(), To);
}

void APathfinderPawn::FindPath(const FTransform From, const FTransform To)
{
	if (Pathfinder && NodeGrid)
	{
		Pathfinder->FindPath(NodeGrid->GetGrid(), From.GetLocation(), To.GetLocation());
	}
}

