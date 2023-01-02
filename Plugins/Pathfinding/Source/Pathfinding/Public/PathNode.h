// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "PathNode.generated.h"

/**
 * 
 */
UCLASS()
class PATHFINDING_API UPathNode : public UObject
{
	GENERATED_BODY()


private:

	UPROPERTY()
	TArray<UPathNode*> Neighbours;

	FVector NodePosition;

	float MovementCost = 0.0f;

	bool IsMovable = true;

public:

	virtual int GetConnectedNeighbourCount(); //

	virtual bool AddNeighbour(UPathNode* Node);

	virtual UPathNode* GetNeighbour(int index);

	virtual bool GetIsNodeMovable();

	virtual void SetIsNodeMovable(bool Val);

	virtual float GetMovementCost();

	virtual FVector GetNodePosition();

	virtual void SetNodePosition(FVector Position);

	virtual bool EmptyNeighbours();

	virtual bool RemoveNeighbour(UPathNode* Node);

	virtual void SetMovementCost(float Cost);

};
