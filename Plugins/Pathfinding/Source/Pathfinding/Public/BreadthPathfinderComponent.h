// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PathfinderComponent.h"
#include "BreadthPathfinderComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent), Blueprintable)
class PATHFINDING_API UBreadthPathfinderComponent : public UPathfinderComponent
{
	GENERATED_BODY()

public:

	UPROPERTY(EditInstanceOnly)
	bool PathFromStartToGoal = true;

public:

	virtual bool FindPath(UNodeGrid* Grid, const FVector& From, const FVector& To) override;
	
};
