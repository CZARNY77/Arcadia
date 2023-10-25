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
	class AGate* gate;

	void PickUpKey();

protected:
	UPROPERTY(EditAnywhere, Category = "Round Settings")
	int countKeys;
	int ownedKeys;
};
