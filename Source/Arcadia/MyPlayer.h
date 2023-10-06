// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyPlayer.generated.h"

//enum potrzebne do zmieniania kierunku chodzenia
enum TurnCamera {
	left,
	right,
	enter
};

UCLASS()
class ARCADIA_API AMyPlayer : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Camera Settings")
	class USpringArmComponent* SpringArm;
	UPROPERTY(EditAnywhere, Category = "Camera Settings")
	class UCameraComponent* Camera;

public:
	AMyPlayer();
	class ACorner* Corner;

protected:
	virtual void BeginPlay() override;

	//Zmienne okreœlaj¹ce kierunek gracza
	TurnCamera turnCamera;
	float X, Y, Yaw;
	int directionTurnCamera;	// okreœla kierynek obrotu kamery
	bool bTurnCamera;

	void ChangeDirection();		// funkcja odpowiada za zmiane kierunku obracania kamery
	void TurnCamera(float dt);	// funkcja obraca kamera

	void ActionKeys();			// funckja z akcjami po klikniêciu
private:
	void Move(float val);

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
