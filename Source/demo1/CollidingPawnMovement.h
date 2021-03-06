// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PawnMovementComponent.h"
#include "CollidingPawnMovement.generated.h"

/**
 * 
 */
UCLASS()
class DEMO1_API UCollidingPawnMovement : public UPawnMovementComponent
{
	GENERATED_BODY()
	
private:
public:
	void DrawRayLine(FVector start, FVector end, float leftTime = 0.0f, FLinearColor color = FLinearColor::Blue);
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;
};
