// Fill out your copyright notice in the Description page of Project Settings.


#include "ChangeCamera.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AChangeCamera::AChangeCamera()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AChangeCamera::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AChangeCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FirstTimeToChangeTime -= DeltaTime;
	if (FirstTimeToChangeTime <= 0.f)
	{
		FirstTimeToChangeTime += TimeToChangeTime;

		auto playerCtrl = UGameplayStatics::GetPlayerController(this,0);
		if (playerCtrl != nullptr)
		{
			if (playerCtrl->GetViewTarget() != Camera1 && Camera1 != nullptr)
			{
				playerCtrl->SetViewTarget(Camera1);
			}
			else if(playerCtrl->GetViewTarget() != Camera2 && Camera2 != nullptr){
				playerCtrl->SetViewTargetWithBlend(Camera2,SmoothBlendTime);
			}
		}
	}
}

