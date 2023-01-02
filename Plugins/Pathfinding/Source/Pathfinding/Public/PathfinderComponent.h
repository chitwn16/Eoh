// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "NodeGrid.h"
#include "PathfinderComponent.generated.h"


UCLASS(Abstract, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class PATHFINDING_API UPathfinderComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPathfinderComponent();

protected:

	/* Was last path search successful*/
	bool HasFoundPath;

public:

	/* Last found path */
	UPROPERTY()
	TArray<UPathNode*> Path;
	/* Last found path as vector */
	UPROPERTY()
	TArray<FVector> PathVector;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual bool FindPath(UNodeGrid* Grid, const FVector& From, const FVector& To);

	/* Was last path search successful*/
	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool GetHasFoundPath() const { return HasFoundPath; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE int32 GetPathNum() const { return PathVector.Num(); }
};
