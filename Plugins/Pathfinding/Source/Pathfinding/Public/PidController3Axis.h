// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "PidController3Axis.generated.h"

/**
 * 
 */
UCLASS(DefaultToInstanced, EditInlineNew, Blueprintable)
class PATHFINDING_API UPidController3Axis : public UObject
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector Kp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector Ki;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector Kd;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector OutputMax;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector OutputMin;

    FVector PreviousError;
    FVector Integral;
    FVector IntegralMax;
    FVector IntegralMin;
	UPROPERTY(VisibleAnywhere)
    FVector Output;

public:

    void SetBounds();

    FVector Cycle(const FVector Previous, const FVector SetPoint, const float Time);
    FVector Cycle(const FVector Error, const float Time);

private:

    FVector Divide(FVector A, FVector B);

    float Inverse(float Value);

    FVector MinMax(FVector Min, FVector Max, FVector Val);
};
