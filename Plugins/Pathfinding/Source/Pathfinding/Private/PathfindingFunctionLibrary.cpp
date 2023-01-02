// Fill out your copyright notice in the Description page of Project Settings.


#include "PathfindingFunctionLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Pathfinding/Public/PriorityQueue.h"
#include "DrawDebugHelpers.h"
#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"

UNodeGrid* UPathfindingFunctionLibrary::GeneratePathNodeGrid(int SizeX, int SizeY, int SizeZ, FVector Center, int Spacing)
{
#if WITH_EDITOR
    if (SizeX % 2 != 0) UE_LOG(LogTemp, Warning, TEXT("Can't generate or update grid. Size must be an even number. Current x: %d"), SizeX);
    if (SizeY % 2 != 0) UE_LOG(LogTemp, Warning, TEXT("Can't generate or update grid. Size must be an even number. Current y: %d"), SizeY);
    if (SizeZ % 2 != 0) UE_LOG(LogTemp, Warning, TEXT("Can't generate or update grid. Size must be an even number. Current z: %d"), SizeZ);
#endif
    
    UNodeGrid* Grid = NewObject<UNodeGrid>();
    Grid->Center = Center;
    Grid->SizeX = SizeX;
    Grid->SizeY = SizeY;
    Grid->SizeZ = SizeZ;
    Grid->Spacing = Spacing;
    Grid->ConstructMap();
    UpdateGrid(Grid, Center, Spacing);
    return Grid;
}

void UPathfindingFunctionLibrary::RemovePhysicsCollisionsFromGrid(UWorld* World, UNodeGrid* Map, float CollisionBoxSize, TArray<TEnumAsByte<EObjectTypeQuery>> Channels, TArray <AActor*>& IgnoreActors, const FCollisionQueryParams& Params)
{
    FCollisionShape CollisionShape;
    CollisionShape.SetBox(FVector3f::One() * CollisionBoxSize);
    FCollisionQueryParams OverlapParams(Params);
    int LengthX = Map->SizeX;
    int LengthY = Map->SizeY;
    int LengthZ = Map->SizeZ;
    FVector Extents = FVector::One() * (CollisionBoxSize / 2 + 1);
    TArray <AActor*> Overlaps;

    for (int X = 0; X < LengthX; X++)
    {
        for (int Y = 0; Y < LengthY; Y++)
        {
            for (int Z = 0; Z < LengthZ; Z++)
            {
                UPathNode* Node = Map->Get(X, Y, Z);
                bool Res = UKismetSystemLibrary::BoxOverlapActors(World, Node->GetNodePosition(), Extents, Channels, nullptr, IgnoreActors, Overlaps);
                if (Res)
                {
                    Map->Set(X, Y, Z, nullptr);
                    DrawDebugBox(
                        World,
                        Node->GetNodePosition(),
                        Extents,
                        FColor::Red,
                        true
                    );
                    UE_LOG(LogTemp, Warning, TEXT("Removing x: %d, y: %d, z: %d"), X, Y, Z);
                }
            }
        }
    }
}

void UPathfindingFunctionLibrary::UpdateGrid(UNodeGrid* Grid, FVector Center, int Spacing)
{
    Grid->Center = Center;
    Grid->Spacing = Spacing;
    int XLength = Grid->SizeX;
    int YLength = Grid->SizeY;
    int ZLength = Grid->SizeZ;
    int HalfX = XLength / 2;
    int HalfY = YLength / 2;
    int HalfZ = ZLength / 2;
#if WITH_EDITOR
    int Nodes = 0;
#endif
    for (int X = -HalfX; X < HalfX; X++)
    {
        for (int Y = -HalfY; Y < HalfY; Y++)
        {
            for (int Z = -HalfZ; Z < HalfZ; Z++)
            {
                FVector position = Center;
                position.X = FMath::RoundToInt(position.X + X * Spacing);
                position.Y = FMath::RoundToInt(position.Y + Y * Spacing);
                position.Z = FMath::RoundToInt(position.Z + Z * Spacing);

                int XP = X + HalfX;
                int YP = Y + HalfY;
                int ZP = Z + HalfZ;
                UPathNode* Node = Grid->Get(XP, YP, ZP);
                if (Node == nullptr)
                {
                    Node = NewObject<UPathNode>();
                    Grid->Set(XP, YP, ZP, Node);
                }
                else
                {
                    Node->EmptyNeighbours();
                }
                Node->SetNodePosition(position);
#if WITH_EDITOR
                Nodes++;
#endif
            }
        }
    }
#if WITH_EDITOR
    UE_LOG(LogTemp, Warning, TEXT("Grid generated. Nodes generated: %d"), Nodes);
#endif
}

void UPathfindingFunctionLibrary::GetPath(UPathNode* Start, UPathNode* Goal, TMap<UPathNode*, UPathNode*> CameFrom, TArray<UPathNode*>& ResultPath, bool PathFromStartToGoal)
{
    ResultPath.Empty();
    UPathNode* Current = Goal;
    while (Current != Start)
    {
        ResultPath.Emplace(Current);
        Current = CameFrom[Current];
    }
    ResultPath.Add(Start);
    if (PathFromStartToGoal)
    {
        Algo::Reverse(ResultPath);
    }
}

bool UPathfindingFunctionLibrary::BreadthFirst(UPathNode* Start, UPathNode* Goal, TArray<UPathNode*>& ResultPath, bool PathFromStartToGoal)
{
	TMap<UPathNode*, UPathNode*> CameFrom;
    TQueue<UPathNode*> Queue;
    ResultPath.Empty();
    Queue.Enqueue(Start);
    CameFrom.Emplace(Start, Start);
    while (!Queue.IsEmpty())
    {
        UPathNode* Current;
        if (Queue.Dequeue(Current))
        {
            // Path found
            if (Current == Goal)
            {
                GetPath(Start, Goal, CameFrom, ResultPath, PathFromStartToGoal);
                Queue.Empty();
                CameFrom.Empty();
                return true;
            }

            int neighbourCount = Current->GetConnectedNeighbourCount();
            for (int i = 0; i < neighbourCount; i++)
            {
                UPathNode* Neighbour = Current->GetNeighbour(i);
                if (Neighbour->GetIsNodeMovable() || CameFrom.Contains(Neighbour))
                {
                    continue;
                }
                Queue.Enqueue(Neighbour);
                CameFrom.Add(Neighbour, Current);
            }
        }
    }
    Queue.Empty();
    CameFrom.Empty();
    // Failed to find a path, clean up
    return false;
}

bool UPathfindingFunctionLibrary::GreedyBestFirstSearch(UPathNode* Start, UPathNode* Goal, TArray<UPathNode*>& ResultPath, bool PathFromStartToGoal)
{
    UPriorityQueue<UPathNode*> Queue = UPriorityQueue<UPathNode*>();
    TMap<UPathNode*, UPathNode*> CameFrom;
    ResultPath.Empty();
    Queue.Enqueue(Start, 0.f);
    CameFrom.Add(Start, Start);
    while (!Queue.IsEmpty())
    {
        // Get first (least cost) item in queue
        UPathNode* CurrentItem = Queue.Dequeue();

        // Path found
        if (CurrentItem == Goal)
        {
            GetPath(Start, Goal, CameFrom, ResultPath, PathFromStartToGoal);
            Queue.Empty();
            CameFrom.Empty();
            return true;
        }
        int NeighbourCount = CurrentItem->GetConnectedNeighbourCount();
        for (int i = 0; i < NeighbourCount; i++)
        {
            UPathNode* Neighbour = CurrentItem->GetNeighbour(i);
            if (Neighbour->GetIsNodeMovable())
            {
                if (!CameFrom.Contains(Neighbour))
                {
                    CameFrom.Add(Neighbour, CurrentItem);
                    float Priority = GetManhattanDistance(CurrentItem, Neighbour);
                    Queue.Enqueue(Neighbour, Priority);
                }
            }
        }
    }
    // Failed to find a path, clean up
    Queue.Empty();
    CameFrom.Empty();
    return false;
}

bool UPathfindingFunctionLibrary::Dijkstra(UPathNode* Start, UPathNode* Goal, TArray<UPathNode*>& ResultPath, bool UseRealDistance, bool UseMovementCost, bool PathFromStartToGoal, bool UseRealDistanceRootCalculation)
{
    UPriorityQueue<UPathNode*> Queue = UPriorityQueue<UPathNode*>();
    TMap<UPathNode*, UPathNode*> CameFrom;
    TMap<UPathNode*, float> CostSoFar;
    ResultPath.Empty();

    Queue.Enqueue(Start, 0.f);
    CameFrom.Add(Start, Start);
    CostSoFar.Add(Start, 0.f);

    while (!Queue.IsEmpty())
    {
        // Get first (least cost) item in queue
        UPathNode* CurrentItem = Queue.Dequeue();

        // Path found
        if (CurrentItem == Goal)
        {
            GetPath(Start, Goal, CameFrom, ResultPath, PathFromStartToGoal);
            Queue.Empty();
            CameFrom.Empty();
            CostSoFar.Empty();
            return true;
        }
        int NeighbourCount = CurrentItem->GetConnectedNeighbourCount();
        for (int i = 0; i < NeighbourCount; i++)
        {
            UPathNode* Neighbour = CurrentItem->GetNeighbour(i);
            if (Neighbour->GetIsNodeMovable())
            {
                float MovementCost = UseMovementCost ? CurrentItem->GetMovementCost() + Neighbour->GetMovementCost() : 0.f;
                float TravelDistance = UseRealDistance ? GetTravelDistance(CurrentItem, Neighbour, UseRealDistanceRootCalculation) : 0.f;
                float NewCost = CostSoFar[CurrentItem] + MovementCost + TravelDistance;
                if (!CostSoFar.Contains(Neighbour))
                {
                    CostSoFar.Add(Neighbour, NewCost);
                    Queue.Enqueue(Neighbour, NewCost);
                    if (!CameFrom.Contains(Neighbour))
                    {
                        CameFrom.Add(Neighbour, CurrentItem);
                    }
                    else
                    {
                        CameFrom[Neighbour] = CurrentItem;
                    }
                }
                else if (NewCost < CostSoFar[Neighbour])
                {
                    CostSoFar[Neighbour] = NewCost;
                    Queue.Enqueue(Neighbour, NewCost);
                    if (!CameFrom.Contains(Neighbour))
                    {
                        CameFrom.Add(Neighbour, CurrentItem);
                    }
                    else
                    {
                        CameFrom[Neighbour] = CurrentItem;
                    }
                }
            }
        }
    }
    // Failed to find a path, clean up
    Queue.Empty();
    CameFrom.Empty();
    CostSoFar.Empty();
    return false;
}

bool UPathfindingFunctionLibrary::AStar(UPathNode* Start, UPathNode* Goal, TArray<UPathNode*>& ResultPath, bool UseRealDistance, bool UseMovementCost, bool PathFromStartToGoal, bool UseRealDistanceRootCalculation)
{
    UPriorityQueue<UPathNode*> Queue = UPriorityQueue<UPathNode*>();
    TMap<UPathNode*, UPathNode*> CameFrom;
    TMap<UPathNode*, float> CostSoFar;
    ResultPath.Empty();

    Queue.Enqueue(Start, 0.f);
    CameFrom.Add(Start, Start);
    CostSoFar.Add(Start, 0.f);

    while (!Queue.IsEmpty())
    {
        // Get first (least cost) item in queue
        UPathNode* CurrentItem = Queue.Dequeue();

        // Path found
        if (CurrentItem == Goal)
        {
            GetPath(Start, Goal, CameFrom, ResultPath, PathFromStartToGoal);
            Queue.Empty();
            CameFrom.Empty();
            CostSoFar.Empty();
            return true;
        }
        int NeighbourCount = CurrentItem->GetConnectedNeighbourCount();
        for (int i = 0; i < NeighbourCount; i++)
        {
            UPathNode* Neighbour = CurrentItem->GetNeighbour(i);
            if (Neighbour && Neighbour->GetIsNodeMovable())
            {
                float MovementCost = UseMovementCost ? CurrentItem->GetMovementCost() + Neighbour->GetMovementCost() : 0.f;
                float TravelDistance = UseRealDistance ? GetTravelDistance(CurrentItem, Neighbour, UseRealDistanceRootCalculation) : 0.f;
                float NewCost = CostSoFar[CurrentItem] + MovementCost + TravelDistance;
                if (!CostSoFar.Contains(Neighbour))
                {
                    CostSoFar.Add(Neighbour, NewCost);
                    float Priority = NewCost + GetManhattanDistance(CurrentItem, Neighbour);
                    Queue.Enqueue(Neighbour, Priority);
                    if (!CameFrom.Contains(Neighbour))
                    {
                        CameFrom.Add(Neighbour, CurrentItem);
                    }
                    else
                    {
                        CameFrom[Neighbour] = CurrentItem;
                    }
                }
                else if (NewCost < CostSoFar[Neighbour])
                {
                    CostSoFar[Neighbour] = NewCost;
                    float Priority = NewCost + GetManhattanDistance(CurrentItem, Neighbour);
                    Queue.Enqueue(Neighbour, Priority);
                    if (!CameFrom.Contains(Neighbour))
                    {
                        CameFrom.Add(Neighbour, CurrentItem);
                    }
                    else
                    {
                        CameFrom[Neighbour] = CurrentItem;
                    }
                }
            }
        }
    }
    // Failed to find a path, clean up
    Queue.Empty();
    CameFrom.Empty();
    CostSoFar.Empty();
    return false;
}

bool UPathfindingFunctionLibrary::ConvertToVectorPath(const FVector& From, const FVector& To, TArray<UPathNode*>& Path, TArray<FVector>& ResultPathVector)
{
    ResultPathVector.Empty();
    int Length = Path.Num() - 1;
    if (Length > 1)
    {
        ResultPathVector.Emplace(From);
        for (int i = 1; i < Length; i++)
        {
            ResultPathVector.Emplace(Path[i]->GetNodePosition());
        }
        ResultPathVector.Emplace(To);
        return true;
    }
    else
    {
        ResultPathVector.Emplace(From);
        ResultPathVector.Emplace(To);
        return false;
    }
}

UPathNode* UPathfindingFunctionLibrary::GetClosestNode(UNodeGrid* Grid, FVector TargetPosition)
{
    int LengthX = Grid->SizeX - 1;
    int LengthY = Grid->SizeY - 1;
    int LengthZ = Grid->SizeZ - 1;
    UPathNode* MaxNode = Grid->Get(LengthX, LengthY, LengthZ);
    // Last node not found for unknown reason
    if (!MaxNode)
    {
        return nullptr;
    }
    float Spacing = Grid->Spacing;
    FVector NodePosition = MaxNode->GetNodePosition();
    int NodeX = LengthX - FMath::RoundToInt((NodePosition.X - TargetPosition.X) / Spacing);
    int NodeY = LengthY - FMath::RoundToInt((NodePosition.Y - TargetPosition.Y) / Spacing);
    int NodeZ = LengthZ - FMath::RoundToInt((NodePosition.Z - TargetPosition.Z) / Spacing);

    if (NodeX < 0) NodeX = 0; else if (NodeX > LengthX) NodeX = LengthX;
    if (NodeY < 0) NodeY = 0; else if (NodeY > LengthY) NodeY = LengthY;
    if (NodeZ < 0) NodeZ = 0; else if (NodeZ > LengthZ) NodeZ = LengthZ;

    // ClosestNode
    UPathNode* Node = Grid->Get(NodeX, NodeY, NodeZ);
    if (Node && Node->GetIsNodeMovable())
    {
        return Node;
    }
    FVector Center = Grid->Center;
    int HalfX = LengthX / 2;
    int HalfY = LengthY / 2;
    int HalfZ = LengthZ / 2;
    for (int X = 0; X < HalfX; X++)
    {
        for (int Y = 0; Y < HalfY; Y++)
        {
            for (int Z = 0; Z < HalfZ; Z++)
            {
                int XP = X + HalfX;
                int YP = Y + HalfY;
                int ZP = Z + HalfZ;
                Node = Grid->Get(XP, YP, ZP);
                if (Node && Node->GetIsNodeMovable())
                {
                    return Node;
                }
                XP = X - HalfX;
                YP = Y - HalfY;
                ZP = Z - HalfZ;
                Node = Grid->Get(XP, YP, ZP);
                if (Node && Node->GetIsNodeMovable())
                {
                    return Node;
                }
            }
        }
    }
    return nullptr;
}

UPathNode* UPathfindingFunctionLibrary::GetNeighbour(UNodeGrid* Grid, int TargetX, int TargetY, int TargetZ)
{
    int LengthX = Grid->SizeX;
    int LengthY = Grid->SizeY;
    int LengthZ = Grid->SizeZ;
    if (TargetX >= 0 && TargetX < LengthX && TargetY >= 0 && TargetY < LengthY && TargetZ >= 0 && TargetZ < LengthZ)
    {
        return Grid->Get(TargetX, TargetY, TargetZ);
    }
    return nullptr;
}

void UPathfindingFunctionLibrary::SetNeighbours(UNodeGrid* Grid)
{
    int LengthX = Grid->SizeX;
    int LengthY = Grid->SizeY;
    int LengthZ = Grid->SizeZ;

    // Set neighbours
    for (int X = 0; X < LengthX; X++)
    {
        for (int Y = 0; Y < LengthY; Y++)
        {
            for (int Z = 0; Z < LengthZ; Z++)
            {
                for (int XNeighbour = X - 1; XNeighbour < X + 2; XNeighbour++)
                {
                    for (int YNeighbour = Y - 1; YNeighbour < Y + 2; YNeighbour++)
                    {
                        for (int ZNeighbour = Z - 1; ZNeighbour < Z + 2; ZNeighbour++)
                        {
                            UPathNode* Node = Grid->Get(X, Y, Z);
                            if (Node)
                            {
                                UPathNode* neighbour = GetNeighbour(Grid, XNeighbour, YNeighbour, ZNeighbour);
                                if (neighbour && neighbour != Node)
                                {
                                    Node->AddNeighbour(neighbour);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

UPathNode* UPathfindingFunctionLibrary::GetCenterGridNode(UNodeGrid* Grid)
{
    int HalfX = Grid->SizeX / 2;
    int HalfY = Grid->SizeY / 2;
    int HalfZ = Grid->SizeZ / 2;
    return Grid->Get(HalfX, HalfY, HalfZ);
}

UPathNode* UPathfindingFunctionLibrary::GetClosestToCenterGridNode(UNodeGrid* Grid)
{
    int LengthX = Grid->SizeX - 1;
    int LengthY = Grid->SizeY - 1;
    int LengthZ = Grid->SizeZ - 1;
    int XNode = LengthX / 2;
    int YNode = LengthY / 2;
    int ZNode = LengthZ / 2;

    UPathNode* Neighbour;
    for (int X = 0; X < LengthX; X++)
    {
        for (int Y = 0; Y < LengthY; Y++)
        {
            for (int Z = 1; Z < LengthZ; Z++)
            {
                Neighbour = GetNeighbour(Grid, XNode + X, YNode + Y, ZNode + Z);
                if (Neighbour)
                {
                    return Neighbour;
                }
            }
        }
    }
    return nullptr;
}

float UPathfindingFunctionLibrary::GetTravelDistance(UPathNode* Current, UPathNode* Goal, bool UseRootCalculation)
{
    // Best result because it uses root to calculate distance
    if (UseRootCalculation)
    {
        return FVector::Dist(Current->GetNodePosition(), Goal->GetNodePosition());
    }
    return FVector::DistSquared(Current->GetNodePosition(), Goal->GetNodePosition());
}

float UPathfindingFunctionLibrary::GetManhattanDistance(UPathNode* Current, UPathNode* Goal)
{
    return GetAbs(Current->GetNodePosition(), Goal->GetNodePosition());
}

float UPathfindingFunctionLibrary::GetAbs(FVector Current, FVector Goal)
{
    return
        FMath::Abs(Current.X - Goal.X) +
        FMath::Abs(Current.Y - Goal.Y) +
        FMath::Abs(Current.Z - Goal.Z)
    ;
}

FQuat UPathfindingFunctionLibrary::CalculateLookRotation(const FVector LookAtLocation, FVector UpDirection)
{
    FVector ForwardDirection = LookAtLocation;
    ForwardDirection = ForwardDirection.GetSafeNormal();
    UpDirection = UpDirection - (ForwardDirection * FVector::DotProduct(UpDirection, ForwardDirection));
    UpDirection = UpDirection.GetSafeNormal();
    FVector Cross1 = FVector::CrossProduct(UpDirection, ForwardDirection);
    FVector Cross2 = FVector::CrossProduct(ForwardDirection, Cross1);
    float m00 = Cross1.X;
    float m01 = Cross1.Y;
    float m02 = Cross1.Z;
    float m10 = Cross2.X;
    float m11 = Cross2.Y;
    float m12 = Cross2.Z;
    float m20 = ForwardDirection.X;
    float m21 = ForwardDirection.Y;
    float m22 = ForwardDirection.Z;
    float num8 = (m00 + m11) + m22;
    FQuat quaternion = FQuat();
    if (num8 > 0.0f)
    {
        float num = FMath::Sqrt(num8 + 1.0f);
        quaternion.W = num * 0.5f;
        num = 0.5f / num;
        quaternion.X = (m12 - m21) * num;
        quaternion.Y = (m20 - m02) * num;
        quaternion.Z = (m01 - m10) * num;
        return quaternion;
    }
    if ((m00 >= m11) && (m00 >= m22))
    {
        float num7 = FMath::Sqrt(((1.0f + m00) - m11) - m22);
        float num4 = 0.5f / num7;
        quaternion.X = 0.5f * num7;
        quaternion.Y = (m01 + m10) * num4;
        quaternion.Z = (m02 + m20) * num4;
        quaternion.W = (m12 - m21) * num4;
        return quaternion;
    }
    if (m11 > m22)
    {
        float num6 = FMath::Sqrt(((1.0f + m11) - m00) - m22);
        float num3 = 0.5f / num6;
        quaternion.X = (m10 + m01) * num3;
        quaternion.Y = 0.5f * num6;
        quaternion.Z = (m21 + m12) * num3;
        quaternion.W = (m20 - m02) * num3;
        return quaternion;
    }
    float num5 = FMath::Sqrt(((1.0f + m22) - m00) - m11);
    float num2 = 0.5f / num5;
    quaternion.X = (m20 + m02) * num2;
    quaternion.Y = (m21 + m12) * num2;
    quaternion.Z = 0.5f * num5;
    quaternion.W = (m01 - m10) * num2;
    return quaternion;
    /*
    FVector newForward = ToPosition - From;
    newForward.Normalize();
    FVector newRight = FVector::CrossProduct(ToPosition, UpDirection);
    FVector newUp = FVector::CrossProduct(newRight, newForward);
    FVector::Rotatio
    return (FTransform(newForward, newRight, UpDirection, From)).Rotator();
    */
}

FRotator UPathfindingFunctionLibrary::CalculateLookRotationRotator(const FVector LookAtLocation, FVector UpDirection)
{
    return FRotator(CalculateLookRotation(LookAtLocation, UpDirection));
}

FVector UPathfindingFunctionLibrary::GetRelativeAngles(const FVector Angle)
{
    FVector Relative = Angle;
    if (Relative.X > 90.f) Relative.X -= 180.f;
    if (Relative.Y > 90.f) Relative.Y -= 180.f;
    if (Relative.Z > 90.f) Relative.Z -= 180.f;
    return Relative;
}

#if WITH_EDITOR
void UPathfindingFunctionLibrary::DrawDebugNodes(UWorld* World, UNodeGrid* Grid, float NodeSize, FColor const& Color, bool PersistentLines, float LifeTime, uint8 DepthPriority, float Thickness)
{
    int LengthX = Grid->SizeX;
    int LengthY = Grid->SizeY;
    int LengthZ = Grid->SizeZ;
    FVector Extents = FVector::One() * (NodeSize / 2 - 1);
    UPathNode* Node;
    for (int X = 0; X < LengthX; X++)
    {
        for (int Y = 0; Y < LengthY; Y++)
        {
            for (int Z = 0; Z < LengthZ; Z++)
            {
                Node = Grid->Get(X, Y, Z);
                if (Node)
                {
                    DrawDebugBox(
                        World,
                        Node->GetNodePosition(),
                        Extents,
                        Color,
                        PersistentLines, 
                        LifeTime,
                        DepthPriority,
                        Thickness
                    );
                }
            }
        }
    }
}
void UPathfindingFunctionLibrary::DrawDebugPath(UWorld* World, TArray<UPathNode*> Path, FColor const& Color, bool PersistentLines, float LifeTime, uint8 DepthPriority, float Thickness)
{
    int Length = Path.Num() - 1;
    for (int i = 0; i < Length; i++)
    {
        DrawDebugLine(
            World,
            Path[i]->GetNodePosition(),
            Path[i + 1]->GetNodePosition(),
            Color,
            PersistentLines,
            LifeTime,
            DepthPriority,
            Thickness
        );
    }
}
void UPathfindingFunctionLibrary::DrawCurrentNodePosition(UWorld* World, UNodeGrid* Grid, float NodeSize, FVector Position, FColor const& Color, bool PersistentLines, float LifeTime, uint8 DepthPriority, float Thickness)
{
    FVector Extents = FVector::One() * (NodeSize / 2 - 1);
    UPathNode* Node = GetClosestNode(Grid, Position);
    if (Node)
    {
        DrawDebugBox(
            World,
            Node->GetNodePosition(),
            Extents,
            Color,
            PersistentLines,
            LifeTime,
            DepthPriority,
            Thickness
        );
    }
}
#endif
