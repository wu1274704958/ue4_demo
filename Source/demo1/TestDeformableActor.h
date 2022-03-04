// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent/Public/ProceduralMeshComponent.h"
#include "TestDeformableActor.generated.h"

UCLASS()
class DEMO1_API ATestDeformableActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATestDeformableActor();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere)
	UProceduralMeshComponent* deformableComp;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void SetMesh(FVector scale = FVector::OneVector,FQuat quat = FQuat::Identity);

};
