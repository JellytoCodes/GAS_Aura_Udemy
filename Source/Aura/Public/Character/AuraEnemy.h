

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/EnemyInterface.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "AuraEnemy.generated.h"

class UWidgetComponent;

UCLASS()
class AURA_API AAuraEnemy 
: public AAuraCharacterBase, public IEnemyInterface
{
	GENERATED_BODY()
	
public :
	AAuraEnemy();

	/** Enemy Interface */
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;
	/** end Enemy Interface */

	/** Combat Interface */
	virtual int32 GetPlayerLevel() override;
	/** end Combat Interface*/

	UPROPERTY(BlueprintAssignable)
	FOnAttribueChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FOnAttribueChangedSignature OnMaxHealthChanged;

protected :
	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo() override;

private :
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Defaults", meta = (AllowPrivateAccess = "true"))
	int32 Level = 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UWidgetComponent> HealthBar;
};
