#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "AuraGameplayAbility.generated.h"

UCLASS()
class AURA_API UAuraGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public :
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	FGameplayTag StartupInputTag;

	virtual FString GetDescription(int32 Level);
	virtual FString GetNextLevelDescription(int32 Level);
	static FString GetLockedDescription(int32 Level);

protected :
	float GetManaCost(float InLevel = 1.f) const;
	float GetCooldown(float InLevel = 1.f) const;
};


/** Note 1. Ability Tags
 *   1) Ability Tags : This ability has these tags
 *    
 *   2) Cancel Abilities with Tag : Abilities with these tags are cancelled when this ability is executed.
 *    
 *   3) Block Abilities with Tag : Abilities with these tags are blocked while this ability is active.
 *    
 *   4) Activation Owned Tags : Tags to apply to activating owner while this ability is active.
 *								These are replicated if ReplicateActivationOwnedTags is enabled in AbilitySystemGlobals.
 *								 
 *   5) Activation Required Tags : This ability can only be activated if the activating actor/component has all of these tags.
 *    
 *   6) Activation Blocked Tags : This ability is blocked if the activating actor/component has any of these tags.
 *    
 *   7) Source Required Tags : This ability can only be activated if the source actor/component has all of these tags.
 *    
 *   8) Source Blocked Tags : This ability is blocked if the source actor/component has any of these tags
 *    
 *   9) Target Required Tags : This ability can only be activated if the target actor/component has all of these tags
 *    
 *   10) Target Blocked Tags : This ability is blocked if the target actor/component has any of these Tags 
 */

/** Note 2. Instancing Policy
 *   1) Instanced Per Actor : A single instance is created for the ability.
 *							   It is reused with each activation.
 * 					Details : Can store persistent data.
 * 					           Variables must be manually reset each time
 * 					           
 * 	 2) Instanced Per Execution : New instance created with each activation
 * 					    Details : Does not store persistent data between activations.
 * 						           Less performant than Instanced per Actor.
 * 						           
 * 	 3) Non-Instanced : Only the Class Default Object is used, no instances are created.
 * 	          Details : Cannot store state, cannot bind to delegates on Ability Tasks.
 * 	                     Best performance of the three options.
 */

/** Note 3. Net Execution Policy
 *   1) Local Only : Only run on the local client, Server does not run the ability.
 *
 *   2) Local Predicted : Activate on the local client, and then on the Server.
 *                        Makes use of prediction.
 *                        Server can roll back invalid changes.
 *
 *   3) Server Only : Only run on the server.
 *
 *   4) Server Initiated : Run on the Server first, then on the owning local client.
 */

/** Note 4. Things Not to Use
 *   1) Replication Policy
 *    : Useless. Don't use It. Refer to Epic's Ability System Questions for an explanation from Epic.
 *      Gameplay Abilities are replicated from Server to owning Client already.
 *      - Note : Gameplay Abilities don't run on Simulated Proxies (use Ges and GCs)
 *
 *   2) Server Respects Remote Ability Cancellation
 *    : Means when the local Client's ability ends, the server's will end
 *      - Not typically a good idea; it's the Server's version that matters
 *
 *   3) Replicate Input Directly
 *    : Always replicates input press/release events to the Server.
 *      - Epic discourages it
 *
 *   Memo. Direct Input state replication. These will be called if bReplicateInputDirectly is true on the ability and
 *         is generally not a good thing to use. (Instead, prefer to use Generic Replicated Events).
 *         UAbilitySystemComponent::ServerSetInputPressed()
 */