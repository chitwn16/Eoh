// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "PathNode.h"
#include "NodeGrid.generated.h"

/**
 * 
 */
UCLASS()
class PATHFINDING_API UNodeGrid : public UObject
{
	GENERATED_BODY()
	
public:

	UPROPERTY()
	TArray<UPathNode*> Map;
	FVector Center;
	int Spacing;
	int SizeX;
	int SizeY;
	int SizeZ;

	FORCEINLINE UPathNode* Get(int X, int Y, int Z)
	{
		return Map[X + Y * SizeX + Z * SizeX * SizeY];
	}

	FORCEINLINE void Set(int X, int Y, int Z, UPathNode* Node)
	{
		Map[X + Y * SizeX + Z * SizeX * SizeY] = Node;
	}

	FORCEINLINE void ConstructMap() 
	{ 
		Map.Init(nullptr, SizeX * SizeY * SizeZ);
	}
};
