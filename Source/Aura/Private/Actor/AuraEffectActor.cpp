
#include "Actor/AuraEffectActor.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemInterface.h"

/** ---------------------------------------------- */

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"

/** ---------------------------------------------- */

AAuraEffectActor::AAuraEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;

	SetRootComponent(CreateDefaultSubobject<USceneComponent>("SceneComponent"));
}

void AAuraEffectActor::BeginPlay()
{
	Super::BeginPlay();
	
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
}

void AAuraEffectActor::OnOverlap(AActor *TargetActor)
{
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
