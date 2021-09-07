// Fill out your copyright notice in the Description page of Project Settings.


#include "CollidingPawn.h"
#include "UObject/ConstructorHelpers.h"
#include "Particles/ParticleSystemComponent.h"

#include "Global.h"


// Sets default values
ACollidingPawn::ACollidingPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//auto root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	//RootComponent = root;
	

	sphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("RootComponent"));
	RootComponent = sphereComp;
	//sphereComp->SetupAttachment(RootComponent);
	sphereComp->SetRelativeLocation(FVector::ZeroVector);
	sphereComp->SetRelativeScale3D(FVector::OneVector);
	sphereComp->SetRelativeRotation(FQuat::Identity);
	sphereComp->InitSphereRadius(40.0f);
	sphereComp->SetCollisionProfileName(TEXT("Pawn"));
	

	auto meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	meshComp->SetupAttachment(sphereComp);
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> mesh(TEXT("/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere"));
	if (mesh.Succeeded())
	{
		meshComp->SetStaticMesh(mesh.Object);
		meshComp->SetRelativeLocation(FVector(0.f,0.f,0.f));
		meshComp->SetRelativeScale3D(FVector(0.8f));
	}

	springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraSpringArm"));
	springArm->SetupAttachment(sphereComp);
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

	if (DeformationVal != 0)
	{
		 float offset = static_cast<float>( DeformationVal) / 100.0f;
		 auto scale = GetActorScale();
		 scale += FVector(offset);
		 if (ToOne && (1.0f - scale.X) <= 0.01f)
		 {
			 ToOne = false;
			 DeformationVal = 0;
			 setWorldSpaceScale(FVector::OneVector);
		 }
		 setWorldSpaceScale(scale);
		 if (scale.Z <= DeformationMin)
		 {
			 setWorldSpaceScale(FVector(DeformationMin + static_cast<float>(1) / 100.0f));
			 energyStorage(DeformationMax);
			 sphereComp->AddForce(FVector::UpVector * jump_force_scale);
		 }else
		 if (scale.Z >= DeformationMax)
		 {
			 setWorldSpaceScale(FVector(DeformationMax - static_cast<float>(1) / 100.0f));
			 DeformationVal = -1;
			 ToOne = true;
		 }
		 
	}
}

// Called to bind functionality to input
void ACollidingPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	APawn::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &ACollidingPawn::moveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACollidingPawn::moveRight);
	PlayerInputComponent->BindAxis("Turn", this, &ACollidingPawn::turn);
	PlayerInputComponent->BindAction("Jump",EInputEvent::IE_Pressed,this,&ACollidingPawn::jump);
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
		sphereComp->AddForce(follow_camera->GetActorForwardVector() * v * force_scale);
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
		sphereComp->AddForce(follow_camera->GetActorRightVector() * v * force_scale);
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

void ACollidingPawn::jump()
{
	if (follow_camera && GetActorLocation().Z <= 40.0f)
	{	
		energyStorage(DeformationMin);
		//sphereComp->AddForce(FVector::UpVector * jump_force_scale);
	}
}

void ACollidingPawn::energyStorage(float to,float s)
{
	float z = GetActorScale3D().Z;
	DeformationVal = static_cast<float>(DeformationUnit) * (to > z ? 1.0f : -1.0f) * s;
}

float ACollidingPawn::isDeformation()
{
	return GetActorScale3D().Z - 1.0f;
}

void ACollidingPawn::setWorldSpaceScale(FVector scale)
{
	auto meshComp = static_cast<UStaticMeshComponent*>(RootComponent->GetChildComponent(0));
	SetActorScale3D(scale);
	auto meshScale = FVector(0.8f, 0.8f, 0.8f) / scale;
	meshComp->SetRelativeScale3D(meshScale);

	auto material = meshComp->GetMaterial(0);
	if (material)
	{
		if(!materialInterface)
			materialInterface = meshComp->CreateDynamicMaterialInstance(0, material);
		materialInterface->SetVectorParameterValue(FName(TEXT("WS_Scale")),FLinearColor(0.0,0.0,-20.0 * ((1.0 - scale.X)* 0.5)));
	}
}