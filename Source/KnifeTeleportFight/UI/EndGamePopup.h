// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/WidgetComponent.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EndGamePopup.generated.h"

UCLASS()
class KNIFETELEPORTFIGHT_API AEndGamePopup : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEndGamePopup();
	void ShowVictoryMessage();
	void ShowDefeatMessage();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	// Components
	UPROPERTY(VisibleAnywhere)
	UWidgetComponent* VictoryPopup;

	UPROPERTY(VisibleAnywhere)
	UWidgetComponent* DefeatPopup;
};
