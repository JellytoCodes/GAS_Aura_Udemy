#pragma once

#include "CoreMinimal.h"
#include "CheckPoint/CheckPoint.h"
#include "MapEntrance.generated.h"

UCLASS()
class AURA_API AMapEntrance : public ACheckPoint
{
	GENERATED_BODY()

public :
	AMapEntrance(const FObjectInitializer& ObjectInitializer);

	/** Highlight Interface */
	virtual void HighlightActor_Implementation() override;
	/** end Highlight Interface */

	/** Save Interface */
	virtual void LoadActor_Implementation() override;
	/** end Save Interface */

	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UWorld> DestinationMap;

	UPROPERTY(EditAnywhere)
	FName DestinationPlayerStartTag;

protected :
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
};
