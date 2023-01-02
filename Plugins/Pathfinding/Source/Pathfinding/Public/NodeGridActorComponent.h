// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PathfinderComponent.h"
#include "NodeGridActorComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PATHFINDING_API UNodeGridActorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UNodeGridActorComponent();

private:

	UPROPERTY()
	UNodeGrid* Grid;

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int GridSizeX = 11;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int GridSizeY = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int GridSizeZ = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Spacing = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float NodeSize = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool DebugDrawGrid;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TEnumAsByte<EObjectTypeQuery>> NodeObjectCollisionChannels;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<AActor*> IgnoreActors;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	//
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	FORCEINLINE UNodeGrid* GetGrid() { return Grid; }

	void GenerateGrid();
};
