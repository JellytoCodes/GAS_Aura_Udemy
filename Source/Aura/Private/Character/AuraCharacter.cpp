
#include "Character/AuraCharacter.h"

/** ---------------------------------------------- */

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

/** ---------------------------------------------- */

AAuraCharacter::AAuraCharacter()
{
	/** Player Camera Setup */
	SpringCamera = CreateDefaultSubobject<USpringArmComponent>("SpringCamera");
	SpringCamera->SetupAttachment(RootComponent);
	SpringCamera->TargetArmLength = 750;
	SpringCamera->SetRelativeRotation(FRotator(-45.f, 0.f, 0.f));
	SpringCamera->bUsePawnControlRotation = false;
	SpringCamera->bEnableCameraLag = true;
	SpringCamera->bInheritPitch = false;
	SpringCamera->bInheritRoll = false;
	SpringCamera->bInheritYaw = false;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>("FollowCamera");
	FollowCamera->SetupAttachment(SpringCamera);

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	/** */
}
