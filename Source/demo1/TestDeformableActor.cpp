// Fill out your copyright notice in the Description page of Project Settings.


#include "TestDeformableActor.h"
#include "Global.h"
// Sets default values
ATestDeformableActor::ATestDeformableActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("root"));
	deformableComp = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("pmc"));
	deformableComp->SetupAttachment(RootComponent);
	deformableComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic,ECollisionResponse::ECR_Block);
	deformableComp->SetSimulatePhysics(true);
	deformableComp->SetEnableGravity(true);
	deformableComp->bUseComplexAsSimpleCollision = false;
	SetMesh(FVector(100.0f,100.0f,100.0f));
}

// Called when the game starts or when spawned
void ATestDeformableActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATestDeformableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void FillLine(TArray<FVector>& arr,float degress,int segmentCount,float height = 0.0f,float offset = 0.5f,bool dropTail = false)
{
	auto seg = 1.0f / segmentCount;
	auto dir = FQuat(FRotator(0,degress,0)) * FVector(0.0,offset,0.0f);
	for(int i = 0;i <= segmentCount;++i)
	{
		auto rotated = FQuat(FRotator(0, degress, 0)) * FVector(seg * i - 0.5f, 0.0f, height);
		auto v = rotated + dir;
		auto size = v.Size();
		if(size > 0.5f)
			v = v.GetUnsafeNormal() * 0.5f; 
		arr.Add(v);
		if(i == segmentCount - 1 && dropTail)
			break;
	}
}

void ATestDeformableActor::SetMesh(FVector scale, FQuat quat)
{
	deformableComp->ClearCollisionConvexMeshes();
	TArray<FVector> Vertices;
	Vertices.Add(FVector(0.0f,0.0f,0.5f));
	FillLine(Vertices,		0,		5,	0.0f);
	FillLine(Vertices,		90, 	5,	0.0f);
	FillLine(Vertices, 		180, 	5,	0.0f);
	FillLine(Vertices, 		270, 	5,	0.0f);
	Vertices.Add(FVector(0.0f,0.0f,-0.5f));
	for (FVector& Vertex : Vertices)
	{
		UE_LOG(MyLog, Warning, TEXT("%f %f %f"), Vertex.X,Vertex.Y,Vertex.Z);
		Vertex *= scale;
	}
	TArray<TArray<FVector>> meshs;
	meshs.Add(Vertices);
	deformableComp->SetCollisionConvexMeshes(meshs);
} 

