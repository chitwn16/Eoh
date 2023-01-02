// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PathNodeInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPathNodeInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PATHFINDING_API IPathNodeInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	/* Can be moved using this node */
	virtual bool GetIsNodeMovable() { return true; }
	virtual int GetConnectedNeighbourCount() = 0;
	virtual bool RemoveNeighbour(IPathNodeInterface* Node) = 0;
	virtual bool AddNeighbour(IPathNodeInterface* Node) = 0;
	virtual IPathNodeInterface* GetNeighbour(int index) = 0;
	virtual void SetMovementCost(float Cost) = 0;
	virtual float GetMovementCost() = 0;
	virtual FVector GetNodePosition() = 0;
	virtual void SetNodePosition(FVector Position) = 0;
};
