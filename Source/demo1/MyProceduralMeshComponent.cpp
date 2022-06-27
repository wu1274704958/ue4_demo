#include "MyProceduralMeshComponent.h"


FPrimitiveSceneProxy* UMyProceduralMeshComponent::CreateSceneProxy()
{
	return nullptr;
}

UBodySetup* UMyProceduralMeshComponent::GetBodySetup()
{
	CreateProcMeshBodySetup();
	return ProcMeshBodySetup;
}

UMaterialInterface* UMyProceduralMeshComponent::GetMaterialFromCollisionFaceIndex(int32 FaceIndex, int32& SectionIndex) const
{
	return  nullptr;
}

bool UMyProceduralMeshComponent::GetPhysicsTriMeshData(FTriMeshCollisionData* CollisionData, bool bInUseAllTriData)
{
	return IInterface_CollisionDataProvider::GetPhysicsTriMeshData(CollisionData, bInUseAllTriData);
}

bool UMyProceduralMeshComponent::ContainsPhysicsTriMeshData(bool bInUseAllTriData) const
{
	return IInterface_CollisionDataProvider::ContainsPhysicsTriMeshData(bInUseAllTriData);
}

bool UMyProceduralMeshComponent::WantsNegXTriMesh()
{
	return false;
}

void UMyProceduralMeshComponent::PostLoad()
{
	Super::PostLoad();
	if(ProcMeshBodySetup && IsTemplate())
	{
		ProcMeshBodySetup->SetFlags(RF_Public | RF_ArchetypeObject);
	}
}

UBodySetup* UMyProceduralMeshComponent::CreateBodySetupHelper() const
{
	UBodySetup* NewBodySetup = NewObject<UBodySetup>(this, NAME_None, (IsTemplate() ? RF_Public | RF_ArchetypeObject : RF_NoFlags));

	NewBodySetup->BodySetupGuid = FGuid::NewGuid();
	NewBodySetup->bGenerateMirroredCollision = false;
	NewBodySetup->bDoubleSidedGeometry = true;
	NewBodySetup->CollisionTraceFlag = CollisionTraceFlag;

	return  NewBodySetup;
}

void UMyProceduralMeshComponent::CreateProcMeshBodySetup()
{
	if(ProcMeshBodySetup == nullptr)
	{
		ProcMeshBodySetup = CreateBodySetupHelper();
	}
}

UMyProceduralMeshComponent::UMyProceduralMeshComponent(const FObjectInitializer& ObjectInitializer) :
	UPrimitiveComponent(ObjectInitializer)
{
	
}

