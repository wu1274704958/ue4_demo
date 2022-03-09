// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ProceduralMeshComponent/Public/ProceduralMeshComponent.h"
#include "UDeformableSphereComp.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DEMO1_API UUDeformableSphereComp : public UProceduralMeshComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UUDeformableSphereComp(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	void GenCubeSphere(int segmentCount);

	TArray<FVector> ConvexMesh;
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void SetMesh(FVector scale = FVector::OneVector, FQuat quat = FQuat::Identity);
	
		
};
