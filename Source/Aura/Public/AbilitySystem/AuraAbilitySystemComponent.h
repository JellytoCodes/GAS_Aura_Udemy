#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AuraAbilitySystemComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FEffectAssetTags, const FGameplayTagContainer& /*Asset Tags*/);

UCLASS()
class AURA_API UAuraAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public :
	void AbilityActorInfoSet();

	FEffectAssetTags EffectAssetTags;

protected :
	void EffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle);

};

/** Note .
 * Init Ability Actor Info
 *  Must be done after possession (the Controller hase been set for the Pawn)
 *  
 *  Player-Controlled Character
 *   - Pawn ¡ê AbilitySystemComponent (ASC Lives on the Pawn)
 *      - Server (Pawn) : PossessedBy
 *      - Client (PlayerController) : AcknowledgePossession
 *   - PlayerState ¡ê AbilitySystemComponent (ASC Lives on the PlayerState)
 *      - Server (Pawn) : PossessedBy
 *      - Client (PlayerController) : OnRep_PlayerState
 * 
 *  AI-Controlled Character
 *   - Pawn ¡ê AbilitySystemComponent (ASC Lives on the Pawn)
 *     - Server / Client (Pawn) : BeginPlay
 */