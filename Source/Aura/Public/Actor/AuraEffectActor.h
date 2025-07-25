#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayEffectTypes.h"
#include "AuraEffectActor.generated.h"

/** ---------------------------------------------- */

class UGameplayEffect;
class UAbilitySystemComponent;

/** ---------------------------------------------- */

UENUM(BlueprintType)
enum class EEffectApplicationPolicy : uint8
{
	ApplyOnOverlap,
	ApplyOnEndOverlap,
	DoNotApply
};

UENUM(BlueprintType)
enum class EEffectRemovalPolicy : uint8
{
	RemoveOnEndOverlap,
	DoNotRemove
};

/** ---------------------------------------------- */

USTRUCT(BlueprintType)
struct FAuraGameplayEffectParams
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UGameplayEffect> GameplayEffectClass = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EEffectApplicationPolicy EffectApplicationPolicy = EEffectApplicationPolicy::DoNotApply;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EEffectRemovalPolicy EffectRemovalPolicy = EEffectRemovalPolicy::DoNotRemove;
};

/** ---------------------------------------------- */

UCLASS()
class AURA_API AAuraEffectActor : public AActor
{
	GENERATED_BODY()
	
public :	
	AAuraEffectActor();

	virtual void Tick(float DeltaSeconds) override;

protected :
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite)
	FVector CalculatedLocation;

	UPROPERTY(BlueprintReadWrite)
	FRotator CalculatedRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup Movement")
	bool bRotates = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup Movement")
	float RotationRate = 45.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup Movement")
	bool bSinusoidalMovement = false;

	UFUNCTION(BlueprintCallable)
	void StartSinusoidalMovement();

	UFUNCTION(BlueprintCallable)
	void StartRotation();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup Movement")
	FVector InitialLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup Movement")
	float SineAmplitude = 8.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup Movement")
	float SinePeriodConstant = 3.f; 

	UFUNCTION(BlueprintCallable)
	void ApplyEffectToTarget(AActor* TargetActor, FAuraGameplayEffectParams& InGameplayEffectParams);

	UFUNCTION(BlueprintCallable)
	void OnOverlap(AActor* TargetActor);

	UFUNCTION(BlueprintCallable)
	void OnEndOverlap(AActor* TargetActor);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Applied Effects")
	TMap<FActiveGameplayEffectHandle, UAbilitySystemComponent*> ActiveEffectHandles;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Applied Effects")
	bool bDestroyOnEffectApplication = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Applied Effects")
	bool bApplyEffectsToEnemies = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	TArray<FAuraGameplayEffectParams> AuraGameplayEffectParams;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Applied Effects")
	float ActorLevel = 1.f;

private :
	float RunningTime = 0.f;

	void ItemMovement(float DeltaSeconds);
};

/** Note 1.
 *  **Gameplay Effects**
 *    1) Data only
 *    2) Don't subclass UGameplayEffect
 *    3) Change Attributes through : Modifiers, Executions
 * 
 *   - Modifier Op : 
 *    1) Add, Multiply, Divide, Override
 * 
 *   - Magnitude Calculation Type :
 *    1) Scalable Float, Attribute Based
 *    2) Custom Calculation Class (MMC), Set by Caller
 * 
 *   - Executions :
 *    1) GameplayEffect Execution Calculation
 * 
 *   - Duration Policy :
 *    1) Instant, Has Duration, Infinite
 *   
 *   - Stacking
 *    1) Add Gameplay Tags
 *    2) Grant Abilities
 */

/** Note 2.
 *  ** Periodic Gameplay Effects **
 *   - Instant :
 *    1) Permanent change to the Base Value
 *  
 *   - Duration & Infinite
 *    1) Change Current Value
 *    2) Undone if/when the effect is removed
 * 
 *   - Periodic
 *    1) Treated like instant - permanently change the Base Value
 * 
 *  ** Stacking **
 *   - Aggregate by Source
 *     
 *   - Aggregate by Target
 */

/** Note 3.
 *   ** Gameplay Tags **
 *    - Gameplay Tag
 *     1) Gameplay Tag Manager
 *     2) Gameplay Tag Container
 *
 *    - IGameplayTagAssetInterface
 *     1) GetOwnedGameplayTags
 *     2) HasMatchingGameplayTag
 *     3) HasAllMatchingGameplayTags
 *     4) HasAnyMatchingGameplayTags
 */

/** Note 4.
 *   ** Attribute Based Modifiers **
 *    - Coefficient(C)
 *    - PreMultiply Additive Value(Pre)
 *    - Post Multiply Additive Value(Post)
 *
 *    (C x (Backing Attribute Value + pre)) + post
 */

/** Note 5.
 *   ** Custom Calculation **
 *    Modifier Magnitude Calculation (MMC)
 */