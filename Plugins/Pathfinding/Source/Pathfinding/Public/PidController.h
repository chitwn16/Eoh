// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "PidController.generated.h"

/**
 * 
 */
UCLASS(DefaultToInstanced, EditInlineNew, Blueprintable)
class PATHFINDING_API UPidController : public UObject
{
	GENERATED_BODY()

public:
	//
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float PFactor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float IFactor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DFactor;

private:
	
    float Integral = 0.0f;
	float LastError = 0.0f;

public:

	float Update(float SetPoint, float Actual, float TimeFrame);
};
