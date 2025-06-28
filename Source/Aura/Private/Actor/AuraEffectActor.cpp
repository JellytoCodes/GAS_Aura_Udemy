
#include "Actor/AuraEffectActor.h"

/** ---------------------------------------------- */

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

void AAuraEffectActor::ApplyEffectToTarget(AActor *TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass)
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

	check(GameplayEffectClass);
	FGameplayEffectContextHandle EffectContextHandle = TargetASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(GameplayEffectClass, 1.f, EffectContextHandle);
	TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
}
