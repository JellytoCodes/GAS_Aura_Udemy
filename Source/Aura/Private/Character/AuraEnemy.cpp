
#include "Character/AuraEnemy.h"

/** ---------------------------------------------- */

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Aura/Aura.h"

/** ---------------------------------------------- */

AAuraEnemy::AAuraEnemy()
{
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

	AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);
	
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

	
}

void AAuraEnemy::BeginPlay()
{
	Super::BeginPlay();

	InitAbilityActorInfo();
}

void AAuraEnemy::InitAbilityActorInfo()
{
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();
}

void AAuraEnemy::HighlightActor()
{
	GetMesh()->SetRenderCustomDepth(true);
	GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
	Weapon->SetRenderCustomDepth(true);
	Weapon->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
}

void AAuraEnemy::UnHighlightActor()
{
	GetMesh()->SetRenderCustomDepth(false);
	Weapon->SetRenderCustomDepth(false);
}
