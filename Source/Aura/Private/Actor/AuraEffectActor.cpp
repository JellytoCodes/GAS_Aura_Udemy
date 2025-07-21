
#include "Actor/AuraEffectActor.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemInterface.h"

/** ---------------------------------------------- */

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Kismet/KismetMathLibrary.h"

/** ---------------------------------------------- */

AAuraEffectActor::AAuraEffectActor()
{
	PrimaryActorTick.bCanEverTick = true;

	SetRootComponent(CreateDefaultSubobject<USceneComponent>("SceneComponent"));
}

void AAuraEffectActor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	RunningTime += DeltaSeconds;
	const float SinePeriod = 2* PI / SinePeriodConstant;
	if (RunningTime > SinePeriod) 
	{
		RunningTime = 0.f;
	}
	ItemMovement(DeltaSeconds);
}

void AAuraEffectActor::BeginPlay()
{
	Super::BeginPlay();
	InitialLocation = GetActorLocation();
	CalculatedLocation = InitialLocation;
	CalculatedRotation = GetActorRotation();
}

void AAuraEffectActor::StartSinusoidalMovement()
{
	bSinusoidalMovement = true;
	InitialLocation = GetActorLocation();
	CalculatedLocation = InitialLocation;
}

void AAuraEffectActor::StartRotation()
{
	bRotates = true;
	CalculatedRotation = GetActorRotation();
}

void AAuraEffectActor::ItemMovement(float DeltaSeconds)
{
	if (bRotates)
	{
		const FRotator DeltaRotation(0.f, DeltaSeconds * RotationRate, 0.f);
		CalculatedRotation = UKismetMathLibrary::ComposeRotators(CalculatedRotation, DeltaRotation);
	}
	if (bSinusoidalMovement)
	{
		const float Sine = SineAmplitude * FMath::Sin(RunningTime * SinePeriodConstant);
		CalculatedLocation = InitialLocation + FVector(0.f, 0.f, Sine);
	}
}

void AAuraEffectActor::ApplyEffectToTarget(AActor* TargetActor, FAuraGameplayEffectParams& InGameplayEffectParams)
{
	/** Interface Cast
	 * auto* ASCInterface = Cast<IAbilitySystemInterface>(Target);
	 * if (ASCInterface)
	 * {
	 *     ASCInterface->GetAbilitySystemComponent();
	 * }
	 *
 	 * Works only when Target explicitly implements IAbilitySystemInterface.
	 * Fast: direct pointer cast, minimal overhead.
	 * Fails on indirect owners (e.g., Pawn → PlayerState). Always null-check.
	 */

	/** UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent()
	 *
	 * Multi-step search:
	 *     1) Target is ASC? return.
	 *     2) Target implements IAbilitySystemInterface? return.
	 *     3) Target is Actor? scan components; if Pawn, follow Owner/PlayerState chain.
	 * Broad coverage, Blueprint-friendly.
	 * Slightly slower due to reflection lookups; still acceptable for general use.
	 */
	
	if (TargetActor->ActorHasTag(FName("Enemy")) && !bApplyEffectsToEnemies) return;

	auto* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if(!TargetASC) return;

	check(InGameplayEffectParams.GameplayEffectClass);
	FGameplayEffectContextHandle EffectContextHandle = TargetASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(InGameplayEffectParams.GameplayEffectClass, ActorLevel, EffectContextHandle);
	FActiveGameplayEffectHandle ActiveEffectHandle = TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());

	const bool bIsInfinite = EffectSpecHandle.Data.Get()->Def.Get()->DurationPolicy == EGameplayEffectDurationType::Infinite;

	if(bIsInfinite && InGameplayEffectParams.EffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap)
	{
		ActiveEffectHandles.Add(ActiveEffectHandle, TargetASC);
	}

	if (bDestroyOnEffectApplication && !bIsInfinite)
	{
		Destroy();
	}
}

void AAuraEffectActor::OnOverlap(AActor *TargetActor)
{
	if (TargetActor->ActorHasTag(FName("Enemy")) && !bApplyEffectsToEnemies) return;

	for(int i = 0 ; i < AuraGameplayEffectParams.Num() ; i++)
	{
		if(AuraGameplayEffectParams[i].EffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
		{
			ApplyEffectToTarget(TargetActor, AuraGameplayEffectParams[i]);
		}
	}
}

void AAuraEffectActor::OnEndOverlap(AActor *TargetActor)
{
	if (TargetActor->ActorHasTag(FName("Enemy")) && !bApplyEffectsToEnemies) return;

	for(int i = 0 ; i < AuraGameplayEffectParams.Num() ; i++)
	{
		if(AuraGameplayEffectParams[i].EffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
		{
			ApplyEffectToTarget(TargetActor, AuraGameplayEffectParams[i]);
		}

		if(AuraGameplayEffectParams[i].EffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap)
		{
			UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
			if(!IsValid(TargetASC)) return;

			TArray<FActiveGameplayEffectHandle> HandlesToRemove;
			for(TTuple<FActiveGameplayEffectHandle, UAbilitySystemComponent*> HandlePair : ActiveEffectHandles)
			{
				if(TargetASC == HandlePair.Value)
				{
					TargetASC->RemoveActiveGameplayEffect(HandlePair.Key, 1);
					HandlesToRemove.Add(HandlePair.Key);
				}
			}

			for(auto& Handle : HandlesToRemove)
			{
				ActiveEffectHandles.FindAndRemoveChecked(Handle);
			}
		}
	}
}
