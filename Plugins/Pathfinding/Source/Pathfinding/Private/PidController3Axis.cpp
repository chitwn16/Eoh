// Fill out your copyright notice in the Description page of Project Settings.


#include "PidController3Axis.h"

void UPidController3Axis::SetBounds()
{
    IntegralMax = Divide(OutputMax, Ki);
    IntegralMin = Divide(OutputMin, Ki);
}

FVector UPidController3Axis::Cycle(FVector Error, float Time)
{
    Integral += Error * Time;
    //Integral = MinMax(IntegralMin, IntegralMax, Integral + (Error * Time));
    const FVector Derivative = (Error - PreviousError) / Time;
    Output = Kp * Error + Ki * Integral + Kd * Derivative;
    PreviousError = Error;
    return Output;
}

FVector UPidController3Axis::Cycle(FVector Previous, FVector SetPoint, float Time)
{
    //const FVector Error = SetPoint - Previous;
    const FVector Error = SetPoint;
    Integral += Error * Time;
    //Integral = MinMax(IntegralMin, IntegralMax, Integral + (Error * Time));

    const FVector Derivative = (Error - PreviousError) / Time;
    Output = Kp * Error + Ki * Integral + Kd * Derivative;
    //Output = MinMax(OutputMin, OutputMax, Output);
    PreviousError = Error;
    return Output;
}

FVector UPidController3Axis::Divide(const FVector A, const FVector B)
{
    const FVector Vector = FVector (Inverse(B.X), Inverse(B.X), Inverse(B.Z));
    return A * Vector;
}

float UPidController3Axis::Inverse(const float Value)
{
    return 1.f / (Value != 0.f ? Value : 1.f);
}

FVector UPidController3Axis::MinMax(const FVector Min, const FVector Max, const FVector Val)
{
    const FVector SelectedMax = Min.ComponentMax(Val);
    const FVector SelectedMin = SelectedMax.ComponentMin(Max);
    return SelectedMin;
}
