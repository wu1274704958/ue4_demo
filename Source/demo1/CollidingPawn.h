// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "CollidingPawnMovement.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include <Camera/CameraActor.h>
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include <tuple>
#include "CollidingPawn.generated.h"


UCLASS()
class DEMO1_API ACollidingPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACollidingPawn();

	UPROPERTY(EditAnyWhere)
	ACameraActor* follow_camera = nullptr;
	UPROPERTY(EditAnyWhere)
	FVector follow_offset = FVector(-179.04f,0.0f,79.1695f);
	UPROPERTY(EditAnyWhere)
	float force_scale= 30000.0f;
	UPROPERTY(EditAnywhere)
	float jump_force_scale = 9000000.0f;
	UPROPERTY(EditAnyWhere)
	float DeformationMin = 0.6f;
	UPROPERTY(EditAnywhere)
	float DeformationMax = 1.1f;
	UPROPERTY(EditAnywhere)
	int DeformationUnit = 1;
	int DeformationVal = 0;
	UPROPERTY(EditAnywhere)
	UCapsuleComponent* sphereComp;
	UPROPERTY(EditAnywhere)
	AActor* BorthPoint;
	UPROPERTY(EditAnywhere)
	float DeadDeep = -1000.0f;

	UPROPERTY(EditAnywhere)
	float cameraPitchOff = 0.0f;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UCollidingPawnMovement* pawnMovement = nullptr;
	USpringArmComponent* springArm;
	UMaterialInstanceDynamic* materialInterface;
	float yaw = 0.f;
	float scale_phy = 1.0f;

	bool ToOne = false;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual UPawnMovementComponent* GetMovementComponent() const override;

	void moveForward(float v);
	void moveRight(float v);
	void turn(float v);
	void jump();
	void setWorldSpaceScale(FVector scale);
	void energyStorage(float to,float s = 1.0f);
	float isDeformation();
	bool rayCastFloor(float offset = 1.0f);
	FVector GetActorScaleEX();
	float GetCameraPitch();
	std::tuple<float,FVector> CalcDegress(FVector a, FVector b);
};
