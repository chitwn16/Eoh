// Fill out your copyright notice in the Description page of Project Settings.


#include "DijkstraPathfinderComponent.h"
#include "PathfindingFunctionLibrary.h"

bool UDijkstraPathfinderComponent::FindPath(UNodeGrid* Grid, const FVector& From, const FVector& To)
{
    UPathNode* FromNode = UPathfindingFunctionLibrary::GetClosestNode(Grid, From);
    UPathNode* ToNode = UPathfindingFunctionLibrary::GetClosestNode(Grid, To);
    HasFoundPath = UPathfindingFunctionLibrary::Dijkstra(
        FromNode,
        ToNode,
        Path,
        UseRealDistance,
        UseMovementCost,
        PathFromStartToGoal,
        UseRealDistanceRootCalculation
    );
    UPathfindingFunctionLibrary::ConvertToVectorPath(From, To, Path, PathVector);
    return HasFoundPath;
}
