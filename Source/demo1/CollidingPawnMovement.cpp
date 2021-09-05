// Fill out your copyright notice in the Description page of Project Settings.


#include "CollidingPawnMovement.h"
#include <Components/LineBatchComponent.h>

void UCollidingPawnMovement::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ȷ���������������Ч���Ա�����ƶ���
	if (!PawnOwner || !UpdatedComponent || ShouldSkipUpdate(DeltaTime))
	{
		return;
	}

	// ��ȡ��Ȼ�������ACollidingPawn::Tick�����õ��ƶ�����
	FVector DesiredMovementThisFrame = ConsumeInputVector().GetClampedToMaxSize(1.0f) * DeltaTime * 150.0f;
	if (!DesiredMovementThisFrame.IsNearlyZero())
	{
		FHitResult Hit;
		SafeMoveUpdatedComponent(DesiredMovementThisFrame, UpdatedComponent->GetComponentRotation(), true, Hit);

		// ��������ײ�����Ի���ȥ
		if (Hit.IsValidBlockingHit())
		{
			auto start = GetActorLocation();
			auto end = start + Hit.Normal * Hit.Distance ;
			auto start2 = Hit.ImpactPoint;
			auto end2 = start2 + Hit.ImpactNormal * 20.f;
			DrawRayLine(start,end,1.f);
			DrawRayLine(start2, end2, 1.f, FLinearColor(1.0f,0.0,0.0,1.0));
			SlideAlongSurface(DesiredMovementThisFrame, 1.f - Hit.Time, Hit.Normal, Hit);
		}
	}
}

void UCollidingPawnMovement::DrawRayLine(FVector start,FVector end,float leftTime,FLinearColor color) 
{
	auto batcher = GetWorld()->PersistentLineBatcher;
	if (batcher)
	{
		batcher->DrawLine(start,end,color,255,1.f,leftTime);
	}

}
