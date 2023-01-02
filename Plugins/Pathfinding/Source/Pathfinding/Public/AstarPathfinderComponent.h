// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PathfinderComponent.h"
#include "AstarPathfinderComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PATHFINDING_API UAstarPathfinderComponent : public UPathfinderComponent
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	bool UseRealDistance = true;
	UPROPERTY(EditAnywhere)
	bool UseMovementCost = false;
	UPROPERTY(EditAnywhere)
	bool PathFromStartToGoal = true;
	UPROPERTY(EditAnywhere)
	bool UseRealDistanceRootCalculation = false;

public:

	virtual bool FindPath(UNodeGrid* Grid, const FVector& From, const FVector& To) override;
	
};
