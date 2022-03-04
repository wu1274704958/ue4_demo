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
	deformableComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn,ECollisionResponse::ECR_Block);
	deformableComp->SetSimulatePhysics(true);
	//deformableComp->SetEnableGravity(true);
	SetMesh(FVector(10.0f,10.0f,10.0f));
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
	deformableComp->ClearAllMeshSections();
	deformableComp->CreateMeshSection()
}

