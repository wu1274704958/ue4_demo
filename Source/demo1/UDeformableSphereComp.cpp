// Fill out your copyright notice in the Description page of Project Settings.


#include "UDeformableSphereComp.h"

// Sets default values for this component's properties
UUDeformableSphereComp::UUDeformableSphereComp(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	//PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UUDeformableSphereComp::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UUDeformableSphereComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void FillLine(TArray<FVector>& arr, float degress, int segmentCount, float height = 0.0f, float offset = 0.5f, bool dropTail = false, bool dropFirst = false)
{
	auto seg = 1.0f / segmentCount;
	auto dir = FQuat(FRotator(0, degress, 0)) * FVector(0.0, offset, 0.0f);
	for (int i = 0; i <= segmentCount; ++i)
	{
		if (dropFirst && i == 0) continue;
		auto rotated = FQuat(FRotator(0, degress, 0)) * FVector(seg * i - 0.5f, 0.0f, height);
		auto v = rotated + dir;
		auto size = v.Size();
		if (size > 0.5f)
			v = v.GetUnsafeNormal() * 0.5f;
		arr.Add(v);
		if (i == segmentCount - 1 && dropTail)
			break;
	}
}

void FillCubeSphereCenter(TArray<FVector>& arr, int segmentCount)
{
	auto seg = 1.0f / segmentCount;
	for (int i = 0; i <= segmentCount; ++i)
	{
		FillLine(arr, 0, segmentCount, seg * i - 0.5f);
		FillLine(arr, 90, segmentCount, seg * i - 0.5f);
		FillLine(arr, 180, segmentCount, seg * i - 0.5f);
		FillLine(arr, 270, segmentCount, seg * i - 0.5f);
	}
}

void FillCubeSpherePlane(TArray<FVector>& arr, int segmentCount, float h)
{
	auto seg = 1.0f / segmentCount;
	for (int i = 0; i <= segmentCount; ++i)
	{
		FillLine(arr, 0, segmentCount, h, seg * i - 0.5f, true, true);
	}
}

void UUDeformableSphereComp::GenCubeSphere(int segmentCount)
{
	ConvexMesh.Reset();
	FillCubeSpherePlane(ConvexMesh, segmentCount, -0.5f);
	FillCubeSphereCenter(ConvexMesh, segmentCount);
	FillCubeSpherePlane(ConvexMesh, segmentCount, 0.5f);
}

void UUDeformableSphereComp::SetMesh(FVector scale, FQuat quat)
{
	TArray<FVector> Vertices;
	if (ConvexMesh.Num() == 0)
		GenCubeSphere(6);
	for (FVector& v : ConvexMesh)
	{
		Vertices.Add(quat.UnrotateVector(v * scale));
	}
	TArray<TArray<FVector>> Meshs;
	Meshs.Add(Vertices);
	SetCollisionConvexMeshes(Meshs);
}
