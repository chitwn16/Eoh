// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "PathfinderAIController.generated.h"

class APathfinderPawn;

/**
 * 
 */
UCLASS()
class PATHFINDING_API APathfinderAIController : public AAIController
{
	GENERATED_BODY()

public:
	APathfinderAIController();

private:

	int CurrentPathPointIndex;

	/* Follow target last position */
	UPROPERTY()
	UPathNode* LastTargetPositionNode;

	UPROPERTY()//
	APathfinderPawn* PathfinderPawn;

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* WorldActorTargetPosition;

protected:

	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

public:
	
};
