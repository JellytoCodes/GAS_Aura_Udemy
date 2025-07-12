#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "AuraHUD.generated.h"

class USpellMenuWidgetController;
class UAttributeMenuWidgetController;
class UAuraUserWidget;
class UOverlayWidgetController;
class UAbilitySystemComponent;
class UAttributeSet;

struct FWidgetControllerParams;

UCLASS()
class AURA_API AAuraHUD : public AHUD
{
	GENERATED_BODY()

public :
	UOverlayWidgetController* GetOverlayWidgetController(const FWidgetControllerParams& WCParams);
	UAttributeMenuWidgetController* GetAttributeMenuWidgetController(const FWidgetControllerParams& WCParams);
	USpellMenuWidgetController* GetSpellMenuWidgetController(const FWidgetControllerParams& WCParams);
	void InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS);

private :
	UPROPERTY()
	TObjectPtr<UAuraUserWidget> OverlayWidget;

	UPROPERTY(EditAnywhere, Category = "WidgetClass", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UAuraUserWidget> OverlayWidgetClass;

	UPROPERTY()
	TObjectPtr<UOverlayWidgetController> OverlayWidgetController;

	UPROPERTY(EditAnywhere, Category = "WidgetClass", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UOverlayWidgetController> OverlayWidgetControllerClass;

	UPROPERTY()
	TObjectPtr<UAttributeMenuWidgetController> AttributeMenuWidgetController;

	UPROPERTY(EditAnywhere, Category = "WidgetClass", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UAttributeMenuWidgetController> AttributeMenuWidgetClass;

	UPROPERTY()
	TObjectPtr<USpellMenuWidgetController> SpellMenuWidgetController;

	UPROPERTY(EditAnywhere, Category = "WidgetClass", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<USpellMenuWidgetController> SpellMenuWidgetClass;
};
