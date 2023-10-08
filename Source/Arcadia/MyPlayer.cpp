// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayer.h"
#include "Corner.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"

// Sets default values
AMyPlayer::AMyPlayer()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("ArmCamera"));
	SpringArm->TargetArmLength = 600.f;
	SpringArm->SetUsingAbsoluteRotation(true);
	SpringArm->bDoCollisionTest = false;
	SpringArm->SocketOffset = FVector(0.f, 0.f, 60.f);
	SpringArm->SetRelativeRotation(FRotator(0.f, 0.f, 0.f));
	SpringArm->bEnableCameraLag = true;
	SpringArm->CameraLagSpeed = 5.f;
	SpringArm->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->bUsePawnControlRotation = false;
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);

	Barrel = CreateDefaultSubobject<USceneComponent>(TEXT("Barrel"));
	Barrel->SetupAttachment(RootComponent);

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);

	X = 0.f; Y = 1.f; Yaw = 0.f;
	bTurnCamera = false;
	bResetRotation = true;
}

void AMyPlayer::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMyPlayer::ChangeDirection()
{
	//Do Yaw przpisuje lokalizacjê jaka ma otrzymac kamera, X i Y sa użyte do zmiany ruchu postaci, directionTurnCamera odpowiada w którym kierunku ma obróci sie kamera.
	//Assigns a location to the Yaw variable that the camera should receive, X and Y are used to change the character's movement, directionTurnCamera is responsible for the direction of camera rotation
	//zrobić lepiej, przekazywać z rogów dane
	//Corner --
	if ((!Corner->AxisX) && (!Corner->AxisY) && (Y == -1)) {
		Yaw = SpringArm->GetComponentRotation().Yaw - 90.f;
		X = -1.f;
		Y = 0.f;
		directionTurnCamera = -1;
	}
	else if ((!Corner->AxisX) && (!Corner->AxisY) && (X == -1)) {
		Yaw = SpringArm->GetComponentRotation().Yaw + 90.f;
		X = 0.f;
		Y = -1.f;
		directionTurnCamera = 1;
	}

	// Corner +-
	if ((Corner->AxisX) && (!Corner->AxisY) && (Y == 1)) {
		Yaw = SpringArm->GetComponentRotation().Yaw + 90.f;
		X = -1.f;
		Y = 0.f;
		directionTurnCamera = 1;
	}
	else if ((Corner->AxisX) && (!Corner->AxisY) && (X == -1)) {
		Yaw = SpringArm->GetComponentRotation().Yaw - 90.f;
		X = 0.f;
		Y = 1.f;
		directionTurnCamera = -1;
	}

	// Corner ++
	if ((Corner->AxisX) && (Corner->AxisY) && (Y == 1)) {
		Yaw = SpringArm->GetComponentRotation().Yaw - 90.f;
		X = 1.f;
		Y = 0.f;
		directionTurnCamera = -1;
	}
	else if ((Corner->AxisX) && (Corner->AxisY) && (X == 1)) {
		Yaw = SpringArm->GetComponentRotation().Yaw + 90.f;
		X = 0.f;
		Y = 1.f;
		directionTurnCamera = 1;
	}

	// Corner -+
	if ((!Corner->AxisX) && (Corner->AxisY) && (Y == -1)) {
		Yaw = SpringArm->GetComponentRotation().Yaw + 90.f;
		X = 1.f;
		Y = 0.f;
		directionTurnCamera = 1;
	}
	else if ((!Corner->AxisX) && (Corner->AxisY) && (X == 1)) {
		Yaw = SpringArm->GetComponentRotation().Yaw - 90.f;
		X = 0.f;
		Y = -1.f;
		directionTurnCamera = -1;
	}
	bTurnCamera = true;
	//wyœrodkowanie postaci
	
	GetCharacterMovement()->Velocity = FVector(0.f, 0.f, 0.f);
	FVector NewLocation = GetCapsuleComponent()->GetRelativeLocation();
	NewLocation.Y = Corner->GetActorLocation().Y;
	NewLocation.X = Corner->GetActorLocation().X;
	GetCapsuleComponent()->SetRelativeLocation(NewLocation);

}

void AMyPlayer::ActionKeys()
{
	if (Corner && !bTurnCamera) {
		ChangeDirection();
	}
}

void AMyPlayer::Shot()
{
	if (Bullet && Barrel) {
		FVector bulletDirection = this->GetActorForwardVector();
		FVector SpawnLocation = Barrel->GetComponentLocation();
		FRotator SpawnRotation = GetActorRotation();
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("B direction: %f %f %f"), bulletDirection.X, bulletDirection.Y, bulletDirection.Z));
		ABullet* NewBullet = GetWorld()->SpawnActor<ABullet>(Bullet, SpawnLocation, SpawnRotation);
	}
}

void AMyPlayer::TurnCamera(float dt)
{
	if (bTurnCamera) {
		float currentR = SpringArm->GetComponentRotation().Yaw;
		if (FMath::IsNearlyEqual(currentR, -180, 2.0f) && bResetRotation) {
			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Yaw: %f"), Yaw));
			Yaw = Yaw > 0 ? -90 : 90;
			bResetRotation = false;
		}
		float addRotation = dt * 90 * directionTurnCamera;
		if (!FMath::IsNearlyEqual(currentR, Yaw, 2.0f)) {
			FRotator newRotation = FRotator(0.f, addRotation, 0.f);
			SpringArm->AddLocalRotation(FQuat(newRotation), false, 0, ETeleportType::None);
		}
		else {
			bTurnCamera = false;
			bResetRotation = true;
		}
			
	}
}

void AMyPlayer::Move(float val)
{
	AddMovementInput(FVector(X, Y, 0.0f), val);
}


void AMyPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TurnCamera(DeltaTime);
}

void AMyPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("Move", this, &AMyPlayer::Move);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("Action", IE_Pressed, this, &AMyPlayer::ActionKeys);
	PlayerInputComponent->BindAction("Shot", IE_Pressed, this, &AMyPlayer::Shot);
}

