// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MyHUD.generated.h"

/**
 * 
 */
UCLASS()
class ARCADIA_API AMyHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintImplementableEvent, Category = "Custom Category")
	void GetDmg(int dmg);
	UFUNCTION(BlueprintImplementableEvent, Category = "Custom Category")
	void PickUpKey();
	UFUNCTION(BlueprintImplementableEvent, Category = "Custom Category")
	void OnCurtain();
	UFUNCTION(BlueprintImplementableEvent, Category = "Custom Category")
	void ExitCurtain();
};
