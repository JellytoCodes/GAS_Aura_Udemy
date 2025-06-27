#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AuraEffectActor.generated.h"

class UGameplayEffect;

UCLASS()
class AURA_API AAuraEffectActor : public AActor
{
	GENERATED_BODY()
	
public :	
	AAuraEffectActor();

protected :
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void ApplyEffectToTarget(AActor* Target, TSubclassOf<UGameplayEffect> GameplayEffectClass);

	UPROPERTY(EditAnywhere, Category = "Applied Effects")
	TSubclassOf<UGameplayEffect> InstantGameplayEffectClass;
	
};

/** Note.
 *  Gameplay Effects
 *   - Data only
 *   - Dont's subclass UGameplayEffect
 *   - Change Attributes through : Modifiers, Executions
 * 
 *  Modifier Op
 *   - Add, Multiply, Divide, Override
 * 
 *  Magnitude Calculation Type
 *   - Scalable Float, Attribute Based
 *   - Custom Calculation Class (MMC), Set by Caller
 * 
 *  Executions
 *   - GameplayEffect Execution Calculation
 * 
 *  Duration Policy
 *   - Instant, Has Duration, Infinite
 *   
 *  Stacking
 *   - Add Gameplay Tags
 *   - Grant Abilities
 * 
 *  Gameplay Effect Spec
 */