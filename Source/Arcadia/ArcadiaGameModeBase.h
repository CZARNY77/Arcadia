// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyPlayer.h"
#include "GameFramework/GameModeBase.h"
#include "ArcadiaGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class ARCADIA_API AArcadiaGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AArcadiaGameModeBase();
	UPROPERTY(EditAnywhere, Category = "Round Settings")
	bool openGate;
	bool bPauseGame;
	class AGate* gate;
	class APlayerController* PlayerController;
	class AMyHUD* MyHUD;
	class AFairy* MyFairy;

	void PickUpKey();
	UFUNCTION(BlueprintCallable)
	FText TextPoints();
	UFUNCTION(BlueprintCallable)
	void PauseGame();
	UFUNCTION(BlueprintCallable)
	void ResumeGame();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Round Settings")
	int countKeys;
	int ownedKeys;
};
