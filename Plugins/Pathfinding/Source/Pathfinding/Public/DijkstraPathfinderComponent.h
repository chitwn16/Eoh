// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PathfinderComponent.h"
#include "DijkstraPathfinderComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent), Blueprintable)
class PATHFINDING_API UDijkstraPathfinderComponent : public UPathfinderComponent
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditInstanceOnly)
	bool UseRealDistance = true;
	UPROPERTY(EditInstanceOnly)
	bool UseMovementCost = false;
	UPROPERTY(EditInstanceOnly)
	bool PathFromStartToGoal = true;
	UPROPERTY(EditInstanceOnly)
	bool UseRealDistanceRootCalculation = false;

public:

	virtual bool FindPath(UNodeGrid* Grid, const FVector& From, const FVector& To) override;
};
