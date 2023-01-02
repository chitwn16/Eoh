// Fill out your copyright notice in the Description page of Project Settings.


#include "PathNode.h"

int UPathNode::GetConnectedNeighbourCount()
{
	return Neighbours.Num();
}

bool UPathNode::AddNeighbour(UPathNode* Node)
{
	if (Neighbours.FindByKey(Node))
	{
		return false;
	}
	Neighbours.Emplace(Node);
	return true;
}

UPathNode* UPathNode::GetNeighbour(int index)
{
	if (index >= 0 && index < Neighbours.Num())
	{
		return Neighbours[index];
	}
	return nullptr;
}

bool UPathNode::GetIsNodeMovable()
{
	return IsMovable;
}

void UPathNode::SetIsNodeMovable(bool Val)
{
	IsMovable = Val;
}

bool UPathNode::RemoveNeighbour(UPathNode* Node)
{
	return Neighbours.Remove(Node) == 1;
}

void UPathNode::SetMovementCost(float Cost)
{
	MovementCost = Cost;
}

float UPathNode::GetMovementCost()
{
	return MovementCost;
}

FVector UPathNode::GetNodePosition()
{
	return NodePosition;
}

void UPathNode::SetNodePosition(FVector Position)
{
	NodePosition = Position;
}

bool UPathNode::EmptyNeighbours()
{
	Neighbours.Empty();
	return true;
}
