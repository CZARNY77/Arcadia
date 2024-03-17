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
	UPROPERTY(EditAnywhere)
	class UBoxComponent* ViewRange;
	UPROPERTY(EditAnywhere, Category = "Custom Category")
	class UAnimMontage* DeathMontage;
	UPROPERTY(EditAnywhere, Category = "Custom Category")
	class UAnimMontage* GetDmgMontage;

public:
	AMyPlayer();
	class ACorner* Corner;
	class ATeleports* Teleport;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom Category")
	bool bCanShot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom Category")
	FRotator ArmRotation;
	UPROPERTY(EditAnywhere, Category = "Custom Category")
	class USceneComponent* TargetAim;

	UFUNCTION(BlueprintCallable, Category = "Custom Category")
	void GetDmg(int dmg);
	UFUNCTION(BlueprintCallable, Category = "Custom Category")
	void Death();


	void AutoNav(FVector TLocation);
	bool isDead();
protected:
	virtual void BeginPlay() override;

	//Zmienne okreœlaj¹ce kierunek gracza
	TurnCamera turnCamera;
	float X, Y, Yaw;
	int directionTurnCamera;	// okreœla kierynek obrotu kamery
	int hp;
	bool bTurnCamera;
	bool bResetRotation;
	bool bCorrect;
	FVector TargetLocation;
	class APlayerController* PlayerController;
	class AEnemy* Enemy;
	class AArcadiaGameModeBase* MyGameMode;

	void ChangeDirection();		// funkcja odpowiada za zmiane kierunku obracania kamery
	void TurnCamera(float dt);	// funkcja obraca kamera
	void SwitchCanShot();
	void Correct();

	void ActionKeys();			// funckja z akcjami po klikniêciu
	void Shot();
	void AutoAim(float dt);
private:
	void Move(float val);

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void OnOverlapBox(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UFUNCTION(BlueprintImplementableEvent, Category = "Custom Category")
	void DeathAnim();
};
