// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Pathfinding/Public/PathNodeInterface.h"
#include "Pathfinding/Public/PathNode.h"
#include "Pathfinding/Public/NodeGrid.h"
#include "PathfindingFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class PATHFINDING_API UPathfindingFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
private:
	static void GetPath(UPathNode* Start, UPathNode* Goal, TMap<UPathNode*, UPathNode*> CameFrom, TArray<UPathNode*>& ResultPath, bool PathFromStartToGoal = true);
	static float GetAbs(FVector Current, FVector Goal);

public:
	
	//UFUNCTION(BlueprintCallable, Category = "Pathfinding Library")

	static UNodeGrid* GeneratePathNodeGrid(int SizeX, int SizeY, int SizeZ, FVector Center, int Spacing);
	
	static void RemovePhysicsCollisionsFromGrid(UWorld* World, UNodeGrid* Map, float CollisionBoxSize, TArray<TEnumAsByte<EObjectTypeQuery>> Channels, TArray <AActor*>& IgnoreActors, const FCollisionQueryParams& Params = FCollisionQueryParams::DefaultQueryParam);
	
	static void UpdateGrid(UNodeGrid* Map, FVector Center, int Spacing);

	static bool BreadthFirst(UPathNode* Start, UPathNode* Goal, TArray<UPathNode*>& ResultPath, bool PathFromStartToGoal = true);

	static bool GreedyBestFirstSearch(UPathNode* Start, UPathNode* Goal, TArray<UPathNode*>& ResultPath, bool PathFromStartToGoal = true);

	static bool Dijkstra(UPathNode* Start, UPathNode* Goal, TArray<UPathNode*>& ResultPath, bool UseRealDistance = true, bool UseMovementCost = false, bool PathFromStartToGoal = true, bool UseRealDistanceRootCalculation = true);

	static bool AStar(UPathNode* Start, UPathNode* Goal, TArray<UPathNode*>& ResultPath, bool UseRealDistance = true, bool UseMovementCost = false, bool PathFromStartToGoal = true, bool UseRealDistanceRootCalculation = true);

	static bool ConvertToVectorPath(const FVector& From, const FVector& To, TArray<UPathNode*>& Path, TArray<FVector>& ResultPathVector);

	static UPathNode* GetClosestNode(UNodeGrid* Grid, FVector TargetPosition);

	static UPathNode* GetNeighbour(UNodeGrid* Grid, const int TargetX, const int TargetY, const int TargetZ);

	static void SetNeighbours(UNodeGrid* Grid);

	static UPathNode* GetCenterGridNode(UNodeGrid* Grid);

	static UPathNode* GetClosestToCenterGridNode(UNodeGrid* Grid);

	static float GetTravelDistance(UPathNode* Current, UPathNode* Goal, bool UseRootCalculation = true);

	static float GetManhattanDistance(UPathNode* Current, UPathNode* Goal);

	/// <summary>
	/// Converted from: https://forums.unrealengine.com/t/posting-the-source-code-for-lookrotation-for-those-who-need-it/111568
	/// </summary>
	/// <param name="LookAtLocation"></param>
	/// <param name="UpDirection"></param>
	/// <returns></returns>
	static FQuat CalculateLookRotation(const FVector LookAtLocation, FVector UpDirection = FVector(0.0f, 0.0f, 1.0f));

	static FRotator CalculateLookRotationRotator(const FVector LookAtLocation, FVector UpDirection = FVector(0.0f, 0.0f, 1.0f));

	/// <summary>
	/// https://forum.unity.com/threads/rigidbody-lookat-torque.146625/#post-2400304
	/// </summary>
	/// <param name="Angle"></param>
	/// <returns></returns>
	/// 
	static FVector GetRelativeAngles(const FVector Angle);
#if WITH_EDITOR
	/// <summary>
	/// Draw ALL nodes in the grid.
	/// Warning, might completely slow down the editor with alot of nodes.
	/// </summary>
	/// <param name="World"></param>
	/// <param name="Grid"></param>
	/// <param name="NodeSize"></param>
	/// <param name="Color"></param>
	/// <param name="bPersistentLines"></param>
	/// <param name="LifeTime"></param>
	/// <param name="DepthPriority"></param>
	/// <param name="Thickness"></param>
	static void DrawDebugNodes(UWorld* World, UNodeGrid * Grid, float NodeSize, FColor const& Color, bool PersistentLines = false, float LifeTime = -1.f, uint8 DepthPriority = 0, float Thickness = 0.f);
	static void DrawDebugPath(UWorld* World, TArray<UPathNode*> Path, FColor const& Color, bool PersistentLines = false, float LifeTime = -1.f, uint8 DepthPriority = 0, float Thickness = 0.f);
	static void DrawCurrentNodePosition(UWorld* World, UNodeGrid* Grid, float NodeSize, FVector Position, FColor const& Color, bool PersistentLines = false, float LifeTime = -1.f, uint8 DepthPriority = 0, float Thickness = 0.f);
#endif
};
