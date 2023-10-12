// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Bullet.h"
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
	UPROPERTY(EditAnywhere, Category = "Shot Settings")
	class USceneComponent* Barrel;
	UPROPERTY(EditAnywhere, Category = "Shot Settings")
	TSubclassOf<class ABullet> Bullet;
public:
	AMyPlayer();
	class ACorner* Corner;
	class ATeleports* Teleport;

protected:
	virtual void BeginPlay() override;

	//Zmienne okreœlaj¹ce kierunek gracza
	TurnCamera turnCamera;
	float X, Y, Yaw;
	int directionTurnCamera;	// okreœla kierynek obrotu kamery
	bool bTurnCamera;
	bool bResetRotation;

	void ChangeDirection();		// funkcja odpowiada za zmiane kierunku obracania kamery
	void TurnCamera(float dt);	// funkcja obraca kamera

	void ActionKeys();			// funckja z akcjami po klikniêciu
	void Shot();
private:
	void Move(float val);

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
