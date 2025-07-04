#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "ExecCalc_Damage.generated.h"

UCLASS()
class AURA_API UExecCalc_Damage : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public :
	UExecCalc_Damage();

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;

private :
	float GetCapturedMagnitude(const FGameplayEffectCustomExecutionParameters& ExecutionParams, const FGameplayEffectAttributeCaptureDefinition& CaptureDefinition, const FAggregatorEvaluateParameters& EvaluateParameters) const;
};

/** Note. Execution Calculation
 *	 Snapshotting (Source)
 *	  - Rules
 *	   1) Snapshotting captures the Attribute value when the Gameplay Effect Spec is created
 *	   2) Not snapshotting captures the Attribute value when the Gameplay Effect is applied
 *	   3) From the Target, the value is captured on Effect Application only
 *	   4) Capture Attributes
 *	   5) Can change more than one Attribute
 *	   6) Can have programmer logic
 *
 *    - Caution
 *	   1) No prediction
 *	   2) Only Instant or Periodic Gameplay Effects
 *	   3) Capturing doesn't run PreAttributeChange; any clamping done there must be done again
 *	   4) Only executed on the Server from Gameplay Abilities with Local Predicted,
 *	      Server Initiated, and Server Only Net Execution Policies
 */