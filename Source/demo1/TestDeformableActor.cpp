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
	deformableComp = CreateDefaultSubobject<UUDeformableSphereComp>(TEXT("pmc"));
	deformableComp->SetupAttachment(RootComponent);
	deformableComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic,ECollisionResponse::ECR_Block);
	deformableComp->SetSimulatePhysics(true);
	deformableComp->SetEnableGravity(true);
	deformableComp->bUseComplexAsSimpleCollision = false;
	deformableComp->SetMesh(FVector(100.0f,100.0f,100.0f),FQuat::Identity);
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
	deformableComp->SetMesh(FVector(100.0f, 100.0f, 70.0f), deformableComp->GetComponentQuat());
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

