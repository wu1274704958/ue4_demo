// Fill out your copyright notice in the Description page of Project Settings.


#include "TestDeformableActor.h"

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

void ATestDeformableActor::SetMesh(FVector scale, FQuat quat)
{
	/*
	deformableComp->ClearAllMeshSections();
	TArray<FVector> Vertices;
	Vertices.Add(FVector(0, 0, 0));
	Vertices.Add(FVector(200, 0, 0));
	Vertices.Add(FVector(0, 0, 100));
	TArray<int> Triangles;
	Triangles.Add(2);
	Triangles.Add(1);
	Triangles.Add(0);
	TArray<FColor> VertexColors;
	VertexColors.Add(FColor(0.0f, 0.0f, 1.0f));
	VertexColors.Add(FColor(1.0f, 0.0f, 0.0f));
	VertexColors.Add(FColor(0.0f, 1.0f, 0.0f));
	TArray<FVector2D> UV;
	UV.Add(FVector2D(0.0f, 0.0f));
	UV.Add(FVector2D(1.0f, 0.0f));
	UV.Add(FVector2D(0.0f, 1.0f));
	TArray<FProcMeshTangent> Tangents;
	Tangents.Init(FProcMeshTangent(1.0f, 0.0f, 0.0f), 3);
	TArray<FVector> Normals;
	Normals.Init(FVector(0, 1.0f, 0), 3);
	deformableComp->CreateMeshSection(0, Vertices, Triangles, Normals,
		UV,UV,UV,UV,
		VertexColors, Tangents, true);
		*/
	deformableComp->ClearAllMeshSections();
	TArray<FVector> Vertices;
	Vertices.Add(FVector(0, 0, 0.5));
	Vertices.Add(FVector(0.5, 0.5, 0));
	Vertices.Add(FVector(0.5, -0.5, 0));
	
	Vertices.Add(FVector(-0.5, 0.5, 0));
	Vertices.Add(FVector(-0.5, -0.5, 0));
	Vertices.Add(FVector(0, 0, -0.5));
	for (FVector& Vertex : Vertices)
	{
		Vertex *= scale;
	}

	TArray<int> Triangles;
	Triangles.Add(0);
	Triangles.Add(1);
	Triangles.Add(2);

	Triangles.Add(0);
	Triangles.Add(4);
	Triangles.Add(3);

	Triangles.Add(0);
	Triangles.Add(3);
	Triangles.Add(1);

	Triangles.Add(0);
	Triangles.Add(2);
	Triangles.Add(4);

	Triangles.Add(1);
	Triangles.Add(3);
	Triangles.Add(2);

	Triangles.Add(2);
	Triangles.Add(3);
	Triangles.Add(4);
	
	/*deformableComp->CreateMeshSection(0, Vertices, Triangles, {},
		{}, {}, {}, {},
		{}, {}, false);*/
	deformableComp->ClearCollisionConvexMeshes();
	TArray<TArray<FVector>> meshs;
	meshs.Add(Vertices);
	deformableComp->SetCollisionConvexMeshes(meshs);
} 

