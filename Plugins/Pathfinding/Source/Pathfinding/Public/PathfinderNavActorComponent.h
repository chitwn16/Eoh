// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Pathfinding/Public/PathfinderComponent.h"
#include "Pathfinding/Public/NodeGridActorComponent.h"
#include "PathfinderNavActorComponent.generated.h"

class UPidController;
class UPidController3Axis;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class PATHFINDING_API UPathfinderNavActorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPathfinderNavActorComponent();

private:

	int32 CurrentPathPointIndex;

	UPROPERTY(meta = (AllowPrivateAccess = "true"))
	UPathfinderComponent* Pathfinder;

	UPROPERTY(meta = (AllowPrivateAccess = "true"))
	UNodeGridActorComponent* NodeGrid;

	bool HasReachedDestination;

	UPROPERTY(meta = (AllowPrivateAccess = "true"))
    UStaticMeshComponent* Body;

public:

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite/*, meta = (UseComponentPicker, AllowedClasses = "UPathfinderComponent") */)
	FComponentReference PathfinderReference;

	/* Moves to next path point if within (n) cm of current path point*/
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	float PathPointReachedDistance = 10.f;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	float MoveSpeed = 1000.f;

	/** Calculates torque (radians) needed to add to look at current path point */
	UPROPERTY(EditAnywhere, Instanced, BlueprintReadWrite)
    UPidController3Axis* PidLookDirection;

	/** Stabilizes angular velocity (radians) to prevent body rotating sporadically*/
    UPROPERTY(EditAnywhere, Instanced, BlueprintReadWrite)
    UPidController3Axis* PidStabilizeAngularVelocity;

	/** Stabilizes body roll axis to prevent upside down flipping */  
    UPROPERTY(EditAnywhere, Instanced, BlueprintReadWrite)
    UPidController3Axis* PidStabilizeRoll;
	

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	/* Add torque using pid to stabilize body rotation */
	void StabilizeBodyAngularVelocity(float DeltaTime);
	/* Add torque using pid to look at current path point direction */
	void LookAtDirection(float DeltaTime, FVector Direction);
	/* try to stabilize roll so object isn't flipped upside down */
	void StabilizeOrientationAxis(float DeltaTime);

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	FCalculateCustomPhysics OnCalculateCustomPhysics;

	/** Used if subscribe to UStaticMeshComponent->BodyInstance custom physics event */
	void PhysicsTick(float DeltaTime, FBodyInstance* BodyInstance);

	void UpdateBehaviour(float DeltaTime);

	UPathfinderComponent* GetPathfinder();

	void CachePathfinder();
	
	void SetGrid(UNodeGridActorComponent* NodeGridActor);

	UFUNCTION(BlueprintCallable)
	void FindPath(const FVector From, const FVector To);

	UFUNCTION(BlueprintCallable)
	FORCEINLINE FVector GetLocation() { return GetOwner()->GetActorLocation(); } const

	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool GetHasReachedDestination() const { return HasReachedDestination; }
};
