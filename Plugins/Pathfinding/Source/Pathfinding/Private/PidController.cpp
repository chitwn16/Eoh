// Fill out your copyright notice in the Description page of Project Settings.


#include "PidController.h"

float UPidController::Update(float SetPoint, float Actual, float TimeFrame)
{
    const float Present = SetPoint - Actual;
    Integral += Present * TimeFrame;
    const float Deriv = (Present - LastError) / TimeFrame;
    LastError = Present;
    return Present * PFactor + Integral * IFactor + Deriv * DFactor;
}
