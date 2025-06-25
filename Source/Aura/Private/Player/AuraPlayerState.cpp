
#include "Player/AuraPlayerState.h"

/** ---------------------------------------------- */

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"

/** ---------------------------------------------- */
AAuraPlayerState::AAuraPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
	
	/** EGameplayEffectReplicationMode
	 *  Replication Mode : Full
	 *   - Use Case : Single Player
	 *   - Description : Gameplay Effects are replicated to all clients
	 *  
	 *	Replication Mode : Mixed
	 *   - Use Case : Multiplayer, Player-Controlled
	 *   - Description : Gameplay Effects are replicated to the owning client only.
	 *     Gameplay Cues and Gameplay Tags replicated to all clients.
	 * 
	 *  Replication Mode : Minimal
	 *   - Use Case : Multiplayer, AI-Controlled 
	 *   - Description : Gameplay Effects are not replicated.
	 *     Gameplay Cues and Gameplay Tags replicated to all clients.
	 * 
	 *  For Mixed Replication Mode : The OwnerActor's Owner must be the Controller.
	 *	ForPawns, this is set automatically in PossessedBy().
	 *  The PlayerState's Owner is automatically set to the Controller.
	 *  Therefore, if your OwnerActor is not the PlayerState, and you use Mixed Replication Mode,
	 *  you must call SetOwner() on the OwnerActor to set its owner to the Controller.
	 */

	AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>("AttributeSet");

	NetUpdateFrequency = 100.f;
}


UAbilitySystemComponent* AAuraPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}