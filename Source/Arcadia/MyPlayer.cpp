// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayer.h"
#include "Corner.h"
#include "Teleports.h"
#include "Enemy.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"

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
	SpringArm->CameraLagSpeed = 3.f;
	SpringArm->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->bUsePawnControlRotation = false;
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);

	Barrel = CreateDefaultSubobject<USceneComponent>(TEXT("Barrel"));
	Barrel->SetupAttachment(GetMesh());

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);

	X = 0.f; Y = 1.f; Yaw = 0.f;
	bTurnCamera = false;
	bResetRotation = true;
	bCanShot = false;
	bCorrect = false;
	
}

void AMyPlayer::Death()
{
}

void AMyPlayer::BeginPlay()
{
	Super::BeginPlay();
	PlayerController = GetWorld()->GetFirstPlayerController();

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AMyPlayer::SwitchCanShot, 4.0f, false);

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
	//wyśrodkowanie postaci
	
	FVector TempLocation = Corner->GetActorLocation();
	AutoNav(TempLocation);
}

void AMyPlayer::ActionKeys()
{
	if (Corner && !bTurnCamera) {
		ChangeDirection();
	}
	if (Teleport) {
		Teleport->TeleportPlayer();
	}
}

void AMyPlayer::Shot()
{

	if (Bullet && bCanShot) {
		FVector weaponLocation = GetMesh()->GetBoneLocation("weapon_l");
		FVector bulletDirection = AutoAim();

		int barrelDirection;
		if(bulletDirection.Y != 0.f && Y != 0.f) barrelDirection = bulletDirection.Y >= 0.f ? 1 : -1;
		if(bulletDirection.X != 0.f && X != 0.f) barrelDirection = bulletDirection.X >= 0.f ? 1 : -1;
		Barrel->SetWorldLocation(weaponLocation + FVector(1.f, 40.f * barrelDirection, 13.f));
		
		FVector SpawnLocation = Barrel->GetComponentLocation();
		FRotator SpawnRotation = GetActorRotation();
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("B direction: %f %f %f"), bulletDirection.X, bulletDirection.Y, bulletDirection.Z));
		
		ABullet* newBullet = GetWorld()->SpawnActor<ABullet>(Bullet, SpawnLocation, SpawnRotation);
		newBullet->SetBulletParameters(bulletDirection, this);
	}
}

FVector AMyPlayer::AutoAim()
{
	FVector StartLocation = GetActorLocation();
	FVector EndLocation = (StartLocation + GetActorForwardVector() * 1000.f) - FVector(70.f*Y, 70.f*X, 0.f);
	FVector bulletDirection = this->GetActorForwardVector();

	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);

	FHitResult HitResult;

	for (int i = 0; i <= 5; i++) {
		bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, CollisionParams);

		if (bHit)
		{
			if (HitResult.GetActor()->ActorHasTag("Enemy")) {
				//DrawDebugLine(GetWorld(), StartLocation, HitResult.ImpactPoint, FColor::Red, false, 1, 0, 1);
				FRotator NewRotation = UKismetMathLibrary::FindLookAtRotation(StartLocation, HitResult.GetActor()->GetActorLocation());
				SetActorRotation(NewRotation);
				bulletDirection -= FVector(0.f, 0.f, 0.1f);
				break;
			}
		}
		EndLocation += FVector(50.f*Y, 50.f*X, 0.f);
	}
	return bulletDirection;
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

void AMyPlayer::SwitchCanShot()
{
	bCanShot = !bCanShot;
}

void AMyPlayer::Correct()
{
	if (bCorrect) {
		FVector Direction = (TargetLocation - GetActorLocation()).GetSafeNormal();

		ACharacter::AddMovementInput(Direction);
		if (FVector::Distance(GetActorLocation(), TargetLocation) < 10.f) {
			bCorrect = false;
			EnableInput(PlayerController);
		}
	}
}

void AMyPlayer::AutoNav(FVector TLocation)
{
	SpringArm->TargetOffset = FVector(0.f, 0.f, 0.f);
	GetCharacterMovement()->Velocity = FVector(0.f, 0.f, 0.f);
	DisableInput(PlayerController);
	bCorrect = true;
	TargetLocation.Z = GetCapsuleComponent()->GetRelativeLocation().Z;
	TargetLocation = TLocation;
}

void AMyPlayer::Move(float val)
{
	AddMovementInput(FVector(X, Y, 0.0f), val);
	SpringArm->TargetOffset = FVector(400 * val * X, 400 * val * Y, 0.f);
}


void AMyPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TurnCamera(DeltaTime);
	Correct();
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

