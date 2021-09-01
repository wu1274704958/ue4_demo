// Fill out your copyright notice in the Description page of Project Settings.


#include "CollidingPawn.h"
#include "UObject/ConstructorHelpers.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/SphereComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

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

	USpringArmComponent* springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraSpringArm"));
	springArm->SetupAttachment(RootComponent);
	springArm->SetRelativeRotation(FRotator(-45.f,0.f,0.f));
	springArm->TargetArmLength = 400.f;
	springArm->bEnableCameraLag = true;
	springArm->CameraLagSpeed = 3.0f;

	auto camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	camera->SetupAttachment(springArm,USpringArmComponent::SocketName);

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
	if (pawnMovement && pawnMovement->UpdatedComponent == RootComponent)
	{
		pawnMovement->AddInputVector(GetActorForwardVector() * v);
	}
}

void ACollidingPawn::moveRight(float v)
{
	if (pawnMovement && pawnMovement->UpdatedComponent == RootComponent)
	{
		pawnMovement->AddInputVector(GetActorRightVector() * v);
	}
}

void ACollidingPawn::turn(float v)
{
	if (pawnMovement && pawnMovement->UpdatedComponent == RootComponent)
	{
		auto rot = GetActorRotation();
		rot.Yaw += v;
		SetActorRotation(rot);
	}
}

