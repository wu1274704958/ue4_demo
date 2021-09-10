// Fill out your copyright notice in the Description page of Project Settings.


#include "CollidingPawn.h"
#include "UObject/ConstructorHelpers.h"
#include "Particles/ParticleSystemComponent.h"
#include "Engine/EngineTypes.h"
#include <Components/LineBatchComponent.h>
#include "GameFramework/PhysicsVolume.h"
#include "Global.h"


// Sets default values
ACollidingPawn::ACollidingPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//auto root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	//RootComponent = root;
	

	sphereComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("RootComponent"));
	RootComponent = sphereComp;
	//sphereComp->SetupAttachment(RootComponent);
	sphereComp->SetRelativeLocation(FVector::ZeroVector);
	sphereComp->SetRelativeScale3D(FVector::OneVector);
	sphereComp->SetRelativeRotation(FQuat::MakeFromEuler(FVector(0.0f,90.0f,0.0f)));
	sphereComp->InitCapsuleSize(50.0f,50.0f);
	sphereComp->SetCollisionProfileName(TEXT("Pawn"));
	sphereComp->SetSimulatePhysics(true);
	sphereComp->SetEnableGravity(true);

	auto meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	meshComp->SetupAttachment(sphereComp);
	static ConstructorHelpers::FObjectFinder<UMaterial> material(TEXT("/Game/StarterContent/Materials/M_Tech_Hex_Tile_2.M_Tech_Hex_Tile_2"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> mesh(TEXT("/Engine/BasicShapes/Sphere.Sphere"));
	if (mesh.Succeeded())
	{
		meshComp->SetStaticMesh(mesh.Object);
		meshComp->SetMaterial(0,material.Object);
		meshComp->SetRelativeLocation(FVector(0.f,0.f,0.f));
		meshComp->SetRelativeScale3D(FVector(1.0f));
		meshComp->SetSimulatePhysics(false);
		meshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
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
		FVector rot = follow_camera->GetActorRotation().Euler();
		rot.Z = yaw;
		
		//UE_LOG(MyLog,Warning,TEXT("%f %f %f"),pos.X,pos.Y,pos.Z);

		auto c_direct = follow_camera->GetActorForwardVector().GetUnsafeNormal();
		auto b_direct = (GetActorLocation() - follow_camera->GetActorLocation()).GetUnsafeNormal();
					
		float z_rot = FMath::RadiansToDegrees( FQuat::MakeFromEuler(FVector(c_direct.X, c_direct.Y,0.0f)).AngularDistance(FQuat::MakeFromEuler( FVector(b_direct.X,b_direct.Y,0.0f))));

		trans.SetRotation(FQuat::MakeFromEuler(FVector(0.0f,0.0f,-z_rot)));
		b_direct = trans.TransformVector(b_direct);

		UE_LOG(MyLog, Warning, TEXT("z_rot %f"), z_rot);
		UE_LOG(MyLog, Warning, TEXT("c_direct %f %f %f"), c_direct.X, c_direct.Y, c_direct.Z);
		UE_LOG(MyLog, Warning, TEXT("b_direct %f %f %f"), b_direct.X, b_direct.Y, b_direct.Z);

		float rad = FMath::RadiansToDegrees(acos(FVector::DotProduct(c_direct, b_direct)));

		auto batcher = GetWorld()->PersistentLineBatcher;
		if (batcher)
		{
			auto b = follow_camera->GetActorLocation();
			auto e = b * c_direct * 10.0;
			batcher->DrawLine(b, e , FLinearColor::White, 255, 1.0f,0.01f);
			batcher->DrawLine(b,b * b_direct * 10.0, FLinearColor::Green, 255, 1.0f,0.01f);
		}

		
		UE_LOG(MyLog, Warning, TEXT("RadiansToDegrees %f %f %f"), rad,rot.Y,follow_camera->GetActorRotation().Pitch);
		
		//rot.Y -= rad;
		trans = FTransform();
		FQuat quat = FQuat::MakeFromEuler(rot);
		trans.SetRotation(quat);
		auto tans_p = trans.TransformPosition(this->follow_offset);
		//rot.Y = rad;
		quat = FQuat::MakeFromEuler(rot);
		follow_camera->SetActorRotation(quat);
		//follow_camera->SetActorLocation(pos + tans_p);
	}

	if (DeformationVal != 0)
	{
		 float offset = static_cast<float>( DeformationVal) / 100.0f;
		 auto scale = GetActorScaleEX();
		 scale += FVector(offset) * DeltaTime;
		 //UE_LOG(MyLog,Warning,TEXT("ToOne %d,abs(1.0f - scale.X) = %f %f"),ToOne, abs(1.0f - scale.X),scale.X);
		 if (ToOne && abs(1.0f - scale.X) <= 0.01f)
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
			 energyStorage(1.0f);
			 ToOne = true;
		 }
	}
	if (GetActorLocation().Z <= DeadDeep)
	{
		if(BorthPoint){
			SetActorLocation(BorthPoint->GetActorLocation());
			sphereComp->GetPhysicsVolume()->Reset();
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
	if (follow_camera && rayCastFloor())
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
	if (follow_camera && rayCastFloor())
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
	UE_LOG(MyLog,Warning,TEXT("abs(scale - 1.0f) %f %d"),abs(isDeformation()),ToOne);
	if (follow_camera && rayCastFloor() && abs(isDeformation()) <= 0.01f)
	{	
		energyStorage(DeformationMin);
		//sphereComp->AddForce(FVector::UpVector * jump_force_scale);
	}
}

void ACollidingPawn::energyStorage(float to,float s)
{
	float z = GetActorScaleEX().Z;
	DeformationVal = static_cast<float>(DeformationUnit) * (to > z ? 1.0f : -1.0f) * s;
}

float ACollidingPawn::isDeformation()
{
	return GetActorScaleEX().Z - 1.0f;
}

bool ACollidingPawn::rayCastFloor(float offset)
{
	auto start = GetActorLocation() + FVector::DownVector * 50.0f;
	auto end = start + FVector::DownVector * offset;
	/*auto batcher = GetWorld()->PersistentLineBatcher;
	if (batcher)
	{
		batcher->DrawLine(start, end, FLinearColor::White, 255, 1.5f);
	}*/
	FCollisionQueryParams queryParam;
	queryParam.AddIgnoredActor(this);
	auto shape = FCollisionShape::MakeSphere(50.0f);
	return GetWorld()->SweepTestByChannel(start,end,FQuat::Identity,ECC_WorldDynamic,shape,queryParam) ||
	GetWorld()->SweepTestByChannel(start, end,FQuat::Identity, ECC_WorldDynamic,shape, queryParam);
}

void ACollidingPawn::setWorldSpaceScale(FVector scale)
{
	auto meshComp = static_cast<UStaticMeshComponent*>(RootComponent->GetChildComponent(0));
	//sphereComp->SetRelativeScale3D(scale);
	//SetActorScale3D(scale);
	scale_phy = scale.X;
	sphereComp->SetCapsuleSize(scale_phy * 50.0f, 50.0f);
	//auto meshScale = FVector(0.8f, 0.8f, 0.8f) / scale;
	//meshComp->SetRelativeScale3D(meshScale);

	auto material = meshComp->GetMaterial(0);
	if (material)
	{
		if(!materialInterface)
			materialInterface = meshComp->CreateDynamicMaterialInstance(0, material);
		materialInterface->SetVectorParameterValue(FName(TEXT("WS_Scale")),FLinearColor(0.0,0.0,-50.0 * ((1.0 - scale.X)* 0.5)));
	}
}

FVector ACollidingPawn::GetActorScaleEX()
{
	return FVector(scale_phy);
}
