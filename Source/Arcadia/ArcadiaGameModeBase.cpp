// Fill out your copyright notice in the Description page of Project Settings.


#include "ArcadiaGameModeBase.h"
#include "Gate.h"

AArcadiaGameModeBase::AArcadiaGameModeBase()
{
	DefaultPawnClass = AMyPlayer::StaticClass();
	openGate = false;
	ownedKeys = 0;
}

void AArcadiaGameModeBase::PickUpKey()
{
	ownedKeys++;
	if (ownedKeys >= countKeys) {
		openGate = true;
		if (gate) gate->OpenGate();
	}
		
}
