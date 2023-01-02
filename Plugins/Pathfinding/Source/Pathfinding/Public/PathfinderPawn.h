// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PathfinderPawn.generated.h"

class UNodeGridActorComponent;
class UPathfinderComponent;
class UPathfinderNavActorComponent;
class UStaticMeshComponent;
class UPidRotationController;

UCLASS()
class PATHFINDING_API APathfinderPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APathfinderPawn();

private:

	float PathUpdateTimeLast;

	UPROPERTY()
	UNodeGridActorComponent* NodeGrid;

	UPROPERTY()
	UPathfinderComponent* Pathfinder;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
    UPathfinderNavActorComponent* NavActor;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
    UStaticMeshComponent* Body;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UPidRotationController* PidRotationController;

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool DebugDrawLastPath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FColor DebugPathColor = FColor::Red;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DebugPathLineThickness = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite/*, meta = (UseComponentPicker, AllowedClasses = "UPathfinderComponent") */)
	FComponentReference PathfinderReference;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* WorldActorTargetPosition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* WorldGrid;

	/* How often to check for new path from self to target world actor. t <= 0 = dont check */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PathUpdateTime = 0.5f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	void CachePathfinder();
	FORCEINLINE UPathfinderComponent* GetPathfinder() const { return Pathfinder; }
	FORCEINLINE UNodeGridActorComponent* GetNodeGrid() const { return NodeGrid; }
	FORCEINLINE UPathfinderNavActorComponent* GetNavActor() const { return NavActor; }
	FORCEINLINE UStaticMeshComponent* GetBody() const { return Body; }
	FORCEINLINE UPidRotationController* GetRotationController() const { return PidRotationController; }

	UFUNCTION(BlueprintCallable)
	void FindPathToTargetWorldActor();
	UFUNCTION(BlueprintCallable)
	void FindPathFromSelf(const FTransform To);
	UFUNCTION(BlueprintCallable)
	void FindPath(const FTransform From, const FTransform To);
};
