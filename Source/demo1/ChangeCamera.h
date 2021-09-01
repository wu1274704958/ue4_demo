// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ChangeCamera.generated.h"

UCLASS()
class DEMO1_API AChangeCamera : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AChangeCamera();

	UPROPERTY(EditAnywhere)
	AActor* Camera1;
	UPROPERTY(EditAnywhere)
	AActor* Camera2;
	UPROPERTY(EditAnywhere)
	float FirstTimeToChangeTime = 1.0f;
	UPROPERTY(EditAnywhere)
	float TimeToChangeTime = 2.0f;
	UPROPERTY(EditAnywhere)
	float SmoothBlendTime = 0.75f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
