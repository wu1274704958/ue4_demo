// Fill out your copyright notice in the Description page of Project Settings.


#include "CollidingPawn.h"
#include "UObject/ConstructorHelpers.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/SphereComponent.h"
#include "Global.h"


// Sets default values
ACollidingPawn::ACollidingPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	auto sphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("RootComponent"));
	RootComponent = sphereComp;
	sphereComp->InitSphereRadius(40.0f);
	sphereComp->SetCollisionProfileName(TEXT("Pawn"));

	auto meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	meshComp->SetupAttachment(RootComponent);
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> mesh(TEXT("/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere"));
	if (mesh.Succeeded())
	{
		meshComp->SetStaticMesh(mesh.Object);
		meshComp->SetRelativeLocation(FVector(0.f,0.f,-40.f));
		meshComp->SetRelativeScale3D(FVector(0.8f));
	}

	springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraSpringArm"));
	springArm->SetupAttachment(RootComponent);
	springArm->SetRelativeRotation(FRotator(0.f,0.f,0.f));
	springArm->TargetArmLength = 0.0;
	springArm->bEnableCameraLag = true;
	springArm->CameraLagSpeed = 3.0f;

	AutoPossessPlayer = EAutoReceiveInput::Player0;

	pawnMovement = CreateDefaultSubobject< UCollidingPawnMovement>(TEXT("Movement"));
	pawnMovement->UpdatedComponent = RootComponent;
	
}

// Called when the game starts or when spawned
void ACollidingPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACollidingPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (follow_camera)
	{
		auto pos = springArm->GetSocketLocation(USpringArmComponent::SocketName);
		FTransform trans;
		FVector rot(0.0,0.0,yaw);
		FQuat quat = FQuat::MakeFromEuler(rot);
		trans.SetRotation(quat);
		auto tans_p = trans.TransformPosition(this->follow_offset);
		//UE_LOG(MyLog,Warning,TEXT("%f %f %f"),pos.X,pos.Y,pos.Z);
		rot.Y = follow_camera->GetActorRotation().Pitch;
		quat = FQuat::MakeFromEuler(rot);
		follow_camera->SetActorRotation(quat);
		follow_camera->SetActorLocation(pos + tans_p);
	}
}

// Called to bind functionality to input
void ACollidingPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	APawn::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &ACollidingPawn::moveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACollidingPawn::moveRight);
	PlayerInputComponent->BindAxis("Turn", this, &ACollidingPawn::turn);
}

UPawnMovementComponent * ACollidingPawn::GetMovementComponent() const 
{
	return pawnMovement;
}

void ACollidingPawn::moveForward(float v)
{
	/*if (pawnMovement && pawnMovement->UpdatedComponent == RootComponent)
	{
		pawnMovement->AddInputVector(GetActorForwardVector() * v);
	}*/
	if (follow_camera)
	{
		((USphereComponent*)RootComponent)->AddForce(follow_camera->GetActorForwardVector() * v * force_scale);
	}
}

void ACollidingPawn::moveRight(float v)
{
	/*if (pawnMovement && pawnMovement->UpdatedComponent == RootComponent)
	{
		pawnMovement->AddInputVector(GetActorRightVector() * v);
	}*/
	if (follow_camera)
	{
		((USphereComponent*)RootComponent)->AddForce(follow_camera->GetActorRightVector() * v * force_scale);
	}
}

void ACollidingPawn::turn(float v)
{
	/*if (pawnMovement && pawnMovement->UpdatedComponent == RootComponent)
	{
		auto rot = GetActorRotation();
		rot.Yaw += v;
		SetActorRotation(rot);
	}*/
	yaw += v;
}

