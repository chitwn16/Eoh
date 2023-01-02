// Fill out your copyright notice in the Description page of Project Settings.


#include "NodeGridActorComponent.h"
#include "Pathfinding/Public/PathfindingFunctionLibrary.h"

// Sets default values for this component's properties
UNodeGridActorComponent::UNodeGridActorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UNodeGridActorComponent::BeginPlay()
{
	Super::BeginPlay();
	GenerateGrid();
}


// Called every frame
void UNodeGridActorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
#if !UE_BUILD_SHIPPING
	if (DebugDrawGrid)
	{
		UPathfindingFunctionLibrary::DrawDebugNodes(GetWorld(), Grid, NodeSize, FColor::Black);
	}
#endif
	// ...
}

void UNodeGridActorComponent::GenerateGrid()
{
	Grid = UPathfindingFunctionLibrary::GeneratePathNodeGrid(GridSizeX, GridSizeY, GridSizeZ, GetOwner()->GetActorLocation(), Spacing);
	UPathfindingFunctionLibrary::RemovePhysicsCollisionsFromGrid(GetWorld(), Grid, NodeSize, NodeObjectCollisionChannels, IgnoreActors);
	UPathfindingFunctionLibrary::SetNeighbours(Grid);
	UE_LOG(LogTemp, Warning, TEXT("Generated grid: %d %d %d"), GridSizeX, GridSizeY, GridSizeZ);
}

