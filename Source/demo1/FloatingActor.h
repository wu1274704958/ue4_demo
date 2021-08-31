// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include <memory>
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FloatingActor.generated.h"


UCLASS()
class DEMO1_API AFloatingActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFloatingActor();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FloatingActor")
		float FloatHeight = 20.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FloatingActor")
		float RotationSpeed = 20.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FloatingActor")
		TSoftObjectPtr<UMaterial> materoalPath = TSoftObjectPtr<UMaterial>(FSoftObjectPath(FName(TEXT("/Game/StarterContent/Materials/M_Tech_Hex_Tile.M_Tech_Hex_Tile"))));
	

	void GrantItemsDeferred();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* mesh;
	std::shared_ptr<ConstructorHelpers::FObjectFinder<UStaticMesh>> static_mesh = nullptr;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
