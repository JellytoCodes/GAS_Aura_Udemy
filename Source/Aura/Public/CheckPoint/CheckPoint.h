

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerStart.h"
#include "CheckPoint.generated.h"

class USphereComponent;

UCLASS()
class AURA_API ACheckPoint : public APlayerStart
{
	GENERATED_BODY()

public :
	ACheckPoint(const FObjectInitializer& ObjectInitializer);

protected :
	UFUNCTION()
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent)
	void CheckPointReached(UMaterialInstanceDynamic* DynamicMaterialInstance);

	void HandleGlowEffects();

private :
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> CheckPointMesh;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> Sphere;

	
};
