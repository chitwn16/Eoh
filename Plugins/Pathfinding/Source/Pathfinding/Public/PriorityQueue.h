// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */

template <class T>
class UPriorityQueueNode 
{
public:
    UPriorityQueueNode<T>();
    UPriorityQueueNode<T>(T InElement, float InPriority);
    bool operator<(const UPriorityQueueNode<T> Other) const;
    T Element = 0;
    float Priority = 0.f;
};


template <class T>
class PATHFINDING_API UPriorityQueue
{
public:
    UPriorityQueue<T>();
private:
    TArray<UPriorityQueueNode<T>> Array;

public:
    /* Always check if IsEmpty() before */
    T Dequeue();
    void Enqueue(T Element, float Priority);
    bool IsEmpty() const;
    void Empty();
};
