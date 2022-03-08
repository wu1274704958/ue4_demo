// Fill out your copyright notice in the Description page of Project Settings.


#include "TestDeformableActor.h"

#include "CollidingPawn.h"
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
	SetMesh(FVector(100.0f,100.0f,100.0f),FQuat::Identity);
	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

// Called when the game starts or when spawned
void ATestDeformableActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATestDeformableActor::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	APawn::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAction("Test", EInputEvent::IE_Pressed, this,  &ATestDeformableActor::TestDeform);
	PlayerInputComponent->BindAxis("MoveForward", this, &ATestDeformableActor::moveForward);
}

void ATestDeformableActor::TestDeform()
{
	UE_LOG(MyLog,Warning,TEXT("Test"));
	SetMesh(FVector(100.0f, 100.0f, 70.0f), deformableComp->GetComponentQuat());
}

void ATestDeformableActor::moveForward(float v)
{
	{
		deformableComp->AddImpulse(GetActorForwardVector() * v * 10.0f * deformableComp->GetMass() * deformableComp->GetMassScale());
	}
}

// Called every frame
void ATestDeformableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void FillLine(TArray<FVector>& arr,float degress,int segmentCount,float height = 0.0f,float offset = 0.5f,bool dropTail = false,bool dropFirst = false)
{
	auto seg = 1.0f / segmentCount;
	auto dir = FQuat(FRotator(0,degress,0)) * FVector(0.0,offset,0.0f);
	for(int i = 0;i <= segmentCount;++i)
	{
		if(dropFirst && i == 0) continue;
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

void FillCubeSpherePlane(TArray<FVector>& arr, int segmentCount,float h)
{
	auto seg = 1.0f / segmentCount;
	for (int i = 0; i <= segmentCount; ++i)
	{
		FillLine(arr, 0, segmentCount, h, seg * i - 0.5f,true,true);
	}
}

void ATestDeformableActor::GenCubeSphere(int segmentCount)
{
	ConvexMesh.Reset();
	FillCubeSpherePlane(ConvexMesh, segmentCount, -0.5f);
	FillCubeSphereCenter(ConvexMesh, segmentCount);
	FillCubeSpherePlane(ConvexMesh, segmentCount, 0.5f);
}

void ATestDeformableActor::SetMesh(FVector scale, FQuat quat)
{
	auto e = quat.Euler();
	UE_LOG(MyLog,Warning,TEXT("Quat %f,%f,%f"),e.X,e.Y,e.Z);
	deformableComp->ClearCollisionConvexMeshes();
	TArray<FVector> Vertices;
	if(ConvexMesh.Num() == 0)
		GenCubeSphere(6);
	for (FVector& v : ConvexMesh)
	{
		Vertices.Add( quat.UnrotateVector(v * scale) );
	}
	deformableComp->AddCollisionConvexMesh(Vertices);
} 

