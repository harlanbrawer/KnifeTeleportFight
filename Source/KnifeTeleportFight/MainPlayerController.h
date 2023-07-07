// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/EndGamePopup.h"

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class KNIFETELEPORTFIGHT_API AMainPlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	virtual void GameHasEnded(class AActor* EndGameFocus = nullptr, bool bIsWinner = false) override;

private:
	// Components
	//UPROPERTY(EditAnywhere)
	//	TSubclassOf<class UUserWidget> HUDClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AEndGamePopup> EndGamePopupClass;

	UPROPERTY(EditAnywhere)
		float RestartDelay = 5;

	FTimerHandle RestartTimer;

	UPROPERTY()
		UUserWidget* HUD;

	AEndGamePopup* EndGamePopup;
};
