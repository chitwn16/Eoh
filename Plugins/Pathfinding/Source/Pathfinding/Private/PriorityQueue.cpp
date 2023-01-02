// Fill out your copyright notice in the Description page of Project Settings.


#include "PriorityQueue.h"


template<class T>
UPriorityQueueNode<T>::UPriorityQueueNode()
{
}

template<typename T>
UPriorityQueueNode<T>::UPriorityQueueNode(T InElement, float InPriority)
{
    Element = InElement;
    Priority = InPriority;
}

template<typename T>
bool UPriorityQueueNode<T>::operator<(const UPriorityQueueNode<T> Other) const
{
    return Priority < Other.Priority;
}

template<typename T>
UPriorityQueue<T>::UPriorityQueue()
{
	Array.Heapify();
}

template<typename T>
T UPriorityQueue<T>::Dequeue()
{
    UPriorityQueueNode<T> Node;
    Array.HeapPop(Node);
    return Node.Element;
}

template<typename T>
void UPriorityQueue<T>::Enqueue(T Element, float Priority)
{
    Array.HeapPush(UPriorityQueueNode<T>(Element, Priority));
}

template<typename T>
bool UPriorityQueue<T>::IsEmpty() const
{
    return Array.Num() == 0;
}

template<typename T>
void UPriorityQueue<T>::Empty()
{
    Array.Empty();
}
