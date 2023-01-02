// Fill out your copyright notice in the Description page of Project Settings.

#include "BreadthPathfinderComponent.h"
#include "PathfindingFunctionLibrary.h"

bool UBreadthPathfinderComponent::FindPath(UNodeGrid* Grid, const FVector& From, const FVector& To)
{
    UPathNode* FromNode = UPathfindingFunctionLibrary::GetClosestNode(Grid, From);
    UPathNode* ToNode = UPathfindingFunctionLibrary::GetClosestNode(Grid, To);
    HasFoundPath = UPathfindingFunctionLibrary::BreadthFirst(
        FromNode,
        ToNode,
        Path,
        PathFromStartToGoal
    );
    UPathfindingFunctionLibrary::ConvertToVectorPath(From, To, Path, PathVector);
    return HasFoundPath;
}
