#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Interfaces/Interface_CollisionDataProvider.h"
#include "Components/PrimitiveComponent.h"
#include "PhysicsEngine/ConvexElem.h"
#include "MyProceduralMeshComponent.generated.h"

UCLASS()
class UMyProceduralMeshComponent : public UPrimitiveComponent,public IInterface_CollisionDataProvider
{
	GENERATED_BODY()
public:

	UMyProceduralMeshComponent(const FObjectInitializer& ObjectInitializer);

	//~ Begin UPrimitiveComponent Interface.
	virtual FPrimitiveSceneProxy* CreateSceneProxy() override;
	virtual class UBodySetup* GetBodySetup() override;
	virtual UMaterialInterface* GetMaterialFromCollisionFaceIndex(int32 FaceIndex, int32& SectionIndex) const override;
	//~ End UPrimitiveComponent Interface.

	//~ Begin Interface_CollisionDataProvider Interface
	virtual bool GetPhysicsTriMeshData(struct FTriMeshCollisionData* CollisionData, bool InUseAllTriData) override;
	virtual bool ContainsPhysicsTriMeshData(bool InUseAllTriData) const override;
	virtual bool WantsNegXTriMesh() override;
	//~ End Interface_CollisionDataProvider Interface

	//~ Begin Uobject Interface
	virtual void PostLoad() override;
	//~ End Uobject Interface

	ECollisionTraceFlag CollisionTraceFlag = ECollisionTraceFlag::CTF_UseComplexAsSimple;

protected:
	UPROPERTY(Instanced)
	TObjectPtr<class UBodySetup> ProcMeshBodySetup;

	UBodySetup* CreateBodySetupHelper() const;
	void CreateProcMeshBodySetup();
};