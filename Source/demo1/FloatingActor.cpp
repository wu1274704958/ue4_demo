// Fill out your copyright notice in the Description page of Project Settings.


#include "FloatingActor.h"
#include "Global.h"



// Sets default values
AFloatingActor::AFloatingActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("mesh"));
	mesh->SetupAttachment(RootComponent);

	static_mesh = std::make_shared<ConstructorHelpers::FObjectFinder<UStaticMesh>>(TEXT("/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube"));
	
	if (static_mesh->Succeeded())
	{
		mesh->SetStaticMesh(static_mesh->Object);
		//mesh->SetMaterial(0, MaterialAsset.Object);
		mesh->SetRelativeLocation(FVector(0.f,0.f,0.f));
	}
	//auto path = materoalPath.ToString();
	//UE_LOG(MyLog,Warning,TEXT("materoalPath.IsNull() %d %p %s"),materoalPath.IsNull(),materoalPath.Get(),*path);
	OnChangeMaterial();
}

// Called when the game starts or when spawned
void AFloatingActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void AFloatingActor::OnChangeMaterial()
{
	if (materoalPath.Get() == nullptr)
	{
		FStreamableManager& mgr = Global::Ins()->GetStreamableManager();
		mgr.RequestAsyncLoad(this->materoalPath.ToSoftObjectPath(), FStreamableDelegate::CreateUObject(this, &AFloatingActor::GrantItemsDeferred));
	}
	else {
		GrantItemsDeferred();
	}
}

// Called every frame
void AFloatingActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FVector NewLocation = GetActorLocation();
	FRotator NewRotation = GetActorRotation();
	float RunningTime = GetGameTimeSinceCreation();
	float DeltaHeight = (FMath::Sin(RunningTime + DeltaTime) - FMath::Sin(RunningTime));
	NewLocation.Z += DeltaHeight * FloatHeight;       //Scale our height by a factor of 20
	float DeltaRotation = DeltaTime * RotationSpeed;    //Rotate by 20 degrees per second
	NewRotation.Yaw += DeltaRotation;
	SetActorLocationAndRotation(NewLocation, NewRotation);
}


void AFloatingActor::PostEditChangeProperty(FPropertyChangedEvent & PropertyChangedEvent)
{
    auto propertyName = PropertyChangedEvent.GetPropertyName();
	if (propertyName == FName(TEXT("materoalPath")))
	{
		auto name = this->materoalPath.ToSoftObjectPath().GetAssetPathString();
		UE_LOG(MyLog, Warning, TEXT("PostEditChangeProperty %s"), *name);
		OnChangeMaterial();
	}
}

void AFloatingActor::GrantItemsDeferred()
{
	UE_LOG(MyLog,Warning,TEXT("Load Success"));
	auto material = materoalPath.Get();
	if(material != NULL)
	{
		mesh->SetMaterial(0,material);
	}
	else {
		UE_LOG(MyLog, Warning, TEXT("Material is Null"));
	}
}
