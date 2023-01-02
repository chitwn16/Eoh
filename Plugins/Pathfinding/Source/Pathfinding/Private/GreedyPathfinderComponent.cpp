// Fill out your copyright notice in the Description page of Project Settings.


#include "GreedyPathfinderComponent.h"
#include "PathfindingFunctionLibrary.h"

bool UGreedyPathfinderComponent::FindPath(UNodeGrid* Grid, const FVector& From, const FVector& To)
{
    UPathNode* FromNode = UPathfindingFunctionLibrary::GetClosestNode(Grid, From);
    UPathNode* ToNode = UPathfindingFunctionLibrary::GetClosestNode(Grid, To);
    HasFoundPath = UPathfindingFunctionLibrary::GreedyBestFirstSearch(
        FromNode,
        ToNode,
        Path,
        PathFromStartToGoal
    );
    UPathfindingFunctionLibrary::ConvertToVectorPath(From, To, Path, PathVector);
    return HasFoundPath;
}
