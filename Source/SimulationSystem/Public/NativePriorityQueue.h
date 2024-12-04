// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
template <typename InElementType>
struct TNativePriorityQueueNode {
	InElementType Element;
	float Priority;

	TNativePriorityQueueNode()
	{
	}

	TNativePriorityQueueNode(InElementType InElement, float InPriority)
	{
		Element = InElement;
		Priority = InPriority;
	}

	bool operator<(const TNativePriorityQueueNode<InElementType> Other) const
	{
		return Priority < Other.Priority;
	}
};

template <typename InElementType>
class SIMULATIONSYSTEM_API TNativePriorityQueue {
public:
	TNativePriorityQueue()
	{
		Array.Heapify();
	}

public:
	// Always check if IsEmpty() before Pop-ing!
	InElementType Pop()
	{
		TNativePriorityQueueNode<InElementType> Node;
		Array.HeapPop(Node);
		return Node.Element;
	}

	TNativePriorityQueueNode<InElementType> PopNode()
	{
		TNativePriorityQueueNode<InElementType> Node;
		Array.HeapPop(Node);
		return Node;
	}

	void Push(InElementType Element, float Priority)
	{
		Array.HeapPush(TNativePriorityQueueNode<InElementType>(Element, Priority));
	}

	bool IsEmpty() const
	{
		return Array.Num() == 0;
	}

	void Refresh()
	{
		Array.HeapSort();
	}

private:
	TArray<TNativePriorityQueueNode<InElementType>> Array;
};