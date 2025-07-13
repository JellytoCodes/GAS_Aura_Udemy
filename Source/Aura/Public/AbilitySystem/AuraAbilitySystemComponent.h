#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AuraAbilitySystemComponent.generated.h"

class UAuraAbilitySystemComponent;

DECLARE_MULTICAST_DELEGATE_OneParam(FEffectAssetTags, const FGameplayTagContainer& /*Asset Tags*/);
DECLARE_MULTICAST_DELEGATE(FAbilitiesGiven);
DECLARE_DELEGATE_OneParam(FForEachAbility, const FGameplayAbilitySpec&);
DECLARE_MULTICAST_DELEGATE_ThreeParams(FAbilityStatusChanged, const FGameplayTag& /* Ability Tag*/, const FGameplayTag& /*Status Tag*/, int32 /* Ability Level*/);
DECLARE_MULTICAST_DELEGATE_FourParams(FAbilityEquipped, const FGameplayTag& /* Ability Tag*/, const FGameplayTag& /*Status Tag*/, const FGameplayTag& /*Slot*/, const FGameplayTag& /*Previous Slot*/)
UCLASS()
class AURA_API UAuraAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public :
	void AbilityActorInfoSet();

	FEffectAssetTags EffectAssetTags;
	FAbilitiesGiven AbilitiesGivenDelegate;
	FAbilityStatusChanged AbilityStatusChanged;
	FAbilityEquipped AbilityEquipped;

	void AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities);
	void AddCharacterPassiveAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupPassiveAbilities);
	bool bStartupAbilitiesGiven = false;

	void AbilityInputTagReleased(const FGameplayTag& InputTag);
	void AbilityInputTagHeld(const FGameplayTag& InputTag);
	void ForEachAbility(const FForEachAbility& Delegate);

	static FGameplayTag GetAbilityTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);
	static FGameplayTag GetInputTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);
	static FGameplayTag GetStatusFromSpec(const FGameplayAbilitySpec& AbilitySpec);

	FGameplayTag GetStatusFromAbilityTag(const FGameplayTag& AbilityTag);
	FGameplayTag GetInputTagFromAbilityTag(const FGameplayTag& AbilityTag);
	FGameplayAbilitySpec* GetSpecFromAbilityTag(const FGameplayTag& AbilityTag);

	void UpgradeAttribute(const FGameplayTag& AttributeTag);

	UFUNCTION(Server, Reliable)
	void ServerUpgradeAttribute(const FGameplayTag& AttributeTag);

	void UpdateAbilityStatuses(int32 Level);

	UFUNCTION(Server, Reliable)
	void ServerSpendSpellPoint(const FGameplayTag& AbilityTag);

	UFUNCTION(Server, Reliable)
	void ServerEquipAbility(const FGameplayTag& AbilityTag, const FGameplayTag& Slot);

	void ClientEquipAbility(const FGameplayTag& AbilityTag, const FGameplayTag& Status, const FGameplayTag& Slot, const FGameplayTag& PreviousSlot);

	bool GetDescriptionsByAbilityTag(const FGameplayTag& AbilityTag, FString& OutDescription, FString& OutNextLevelDescription);

	void ClearSlot(FGameplayAbilitySpec* Spec);
	void ClearAbilitiesSlot(const FGameplayTag& Slot);
	static bool AbilityHasSlot(FGameplayAbilitySpec* Spec, const FGameplayTag& Slot);

protected :

	virtual void OnRep_ActivateAbilities() override;

	UFUNCTION(Client, Reliable)
	void ClineEffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle);

	UFUNCTION(Client, Reliable)
	void ClientUpdateAbilityStatus(const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, int32 AbilityLevel);
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

