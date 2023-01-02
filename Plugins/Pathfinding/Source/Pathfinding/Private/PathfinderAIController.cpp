// Fill out your copyright notice in the Description page of Project Settings.


#include "PathfinderAIController.h"
#include "Pathfinding/Public/PathfinderPawn.h"
#include "Pathfinding/Public/NodeGridActorComponent.h"
#include "Pathfinding/Public/PathfinderNavActorComponent.h"
#include "Pathfinding/Public/PathfindingFunctionLibrary.h"

APathfinderAIController::APathfinderAIController()
{
    PrimaryActorTick.bCanEverTick = true;
    PrimaryActorTick.bStartWithTickEnabled = true;
}

void APathfinderAIController::BeginPlay()
{
    Super::BeginPlay();
    PathfinderPawn = Cast<APathfinderPawn>(GetPawn());
}

void APathfinderAIController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    if (!WorldActorTargetPosition)
    {
        return;
    }
    if (PathfinderPawn && PathfinderPawn->GetNodeGrid() && PathfinderPawn->GetNavActor())
    {
        UPathNode* FromNode = UPathfindingFunctionLibrary::GetClosestNode(PathfinderPawn->GetNodeGrid()->GetGrid(), WorldActorTargetPosition->GetActorLocation());
        // not same position to target
        // try to find path
        if (FromNode && FromNode != LastTargetPositionNode)
        {
            PathfinderPawn->GetNavActor()->FindPath(PathfinderPawn->GetActorLocation(), WorldActorTargetPosition->GetActorLocation());
            //LastTargetPositionNode = FromNode;
        }
    }
}
