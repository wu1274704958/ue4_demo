// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UDeformableSphereComp.h"
#include "TestDeformableActor.generated.h"

UCLASS()
class DEMO1_API ATestDeformableActor : public APawn
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATestDeformableActor();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere)
	UUDeformableSphereComp* deformableComp;
	TArray<FVector> ConvexMesh;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	void TestDeform();
	void moveForward(float v);
};
