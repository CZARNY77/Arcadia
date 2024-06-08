// Fill out your copyright notice in the Description page of Project Settings.


#include "ArcadiaGameModeBase.h"
#include "Gate.h"
#include "MyHUD.h"
#include "Fairy.h"
#include "Kismet/GameplayStatics.h"

AArcadiaGameModeBase::AArcadiaGameModeBase()
{
	DefaultPawnClass = AMyPlayer::StaticClass();
	openGate = false;
	ownedKeys = 0;
}

void AArcadiaGameModeBase::BeginPlay()
{
	PlayerController = GetWorld()->GetFirstPlayerController();
	AHUD* Hud = PlayerController->GetHUD();
	MyHUD = Cast<AMyHUD>(Hud);

	//if (GetWorld()->GetName() == "Menu")	DisableInput(PlayerController);
}

void AArcadiaGameModeBase::PickUpKey()
{
	ownedKeys++;
	if (MyHUD) MyHUD->PickUpKey();
	if (ownedKeys >= countKeys) {
		openGate = true;
		if (gate) gate->OpenGate();
	}

	if (MyFairy)	MyFairy->AddSystemLog("Gracz zdoby³ klucz. Brama siê otworzy³a.");
}

FText AArcadiaGameModeBase::TextPoints()
{
	FString temp = FString::Printf(TEXT("%d/%d"), ownedKeys, countKeys);
	return FText::FromString(temp);
}

void AArcadiaGameModeBase::PauseGame()
{
	UGameplayStatics::SetGamePaused(GetWorld(), true);
}

void AArcadiaGameModeBase::ResumeGame()
{
	UGameplayStatics::SetGamePaused(GetWorld(), false);

}
