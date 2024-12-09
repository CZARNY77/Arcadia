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
#include "MyHUD.h"
#include "Kismet/GameplayStatics.h"
#include "ArcadiaGameModeBase.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"

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

	ViewRange = CreateDefaultSubobject<UBoxComponent>(TEXT("ViewRange"));
	ViewRange->InitBoxExtent(FVector(100, 50, 50));
	ViewRange->SetCollisionProfileName("Trigger");
	ViewRange->SetupAttachment(RootComponent);

	TargetAim = CreateDefaultSubobject<USceneComponent>(TEXT("TargetAim"));
	TargetAim->SetupAttachment(RootComponent);

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);

	X = 0.f; Y = 1.f; Yaw = 0.f;
	bTurnCamera = false;
	bResetRotation = true;
	bCanShot = false;
	bCorrect = false;
	hp = 5;
	
}

void AMyPlayer::GetDmg(int dmg)
{

	hp -= dmg;
	if (MyGameMode->MyHUD)	MyGameMode->MyHUD->GetDmg(dmg);;
	if (hp <= 0) {
		DisableInput(PlayerController);
		if (DeathMontage) {
			PlayAnimMontage(DeathMontage);
			return;
		}
	}
	PlayAnimMontage(GetDmgMontage);
}

void AMyPlayer::Death()
{
	FName LevelName = FName(*GetWorld()->GetAuthGameMode<AArcadiaGameModeBase>()->LevelName);
	UGameplayStatics::OpenLevel(this, LevelName);
}

void AMyPlayer::BeginPlay()
{
	Super::BeginPlay();
	MyGameMode = Cast<AArcadiaGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	PlayerController = GetWorld()->GetFirstPlayerController();
	DisableInput(PlayerController);
	
	ViewRange->OnComponentBeginOverlap.AddDynamic(this, &AMyPlayer::OnOverlapBox);
	ViewRange->OnComponentEndOverlap.AddDynamic(this, &AMyPlayer::OnOverlapEnd);

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AMyPlayer::SwitchCanShot, 4.0f, false);

}

void AMyPlayer::ChangeDirection()
{
	//Do Yaw przpisuje lokalizacjê jaka ma otrzymac kamera, X i Y sa użyte do zmiany ruchu postaci, directionTurnCamera odpowiada w którym kierunku ma obróci sie kamera.
	//Assigns a location to the Yaw variable that the camera should receive, X and Y are used to change the character's movement, directionTurnCamera is responsible for the direction of camera rotation

	// Corner --
	if ((!Corner->AxisX) && (!Corner->AxisY) && (abs(Y) == 1)) {
		Yaw = SpringArm->GetComponentRotation().Yaw - 90.f;
		X = Y;
		Y = 0.f;
		directionTurnCamera = -1;
	}
	else if ((!Corner->AxisX) && (!Corner->AxisY) && (abs(X) == 1)) {
		Yaw = SpringArm->GetComponentRotation().Yaw + 90.f;
		Y = X;
		X = 0.f;
		directionTurnCamera = 1;
	}

	// Corner +-
	if ((Corner->AxisX) && (!Corner->AxisY) && (abs(Y) == 1)) {
		Yaw = SpringArm->GetComponentRotation().Yaw + 90.f;
		X = -Y;
		Y = 0.f;
		directionTurnCamera = 1;
		bResetRotation = true;
	}
	else if ((Corner->AxisX) && (!Corner->AxisY) && (abs(X) == 1)) {
		Yaw = SpringArm->GetComponentRotation().Yaw - 90.f;
		Y = -X;
		X = 0.f;
		directionTurnCamera = -1;
	}

	// Corner ++
	if ((Corner->AxisX) && (Corner->AxisY) && (abs(Y) == 1)) {
		Yaw = SpringArm->GetComponentRotation().Yaw - 90.f;
		X = Y;
		Y = 0.f;
		directionTurnCamera = -1;
	}
	else if ((Corner->AxisX) && (Corner->AxisY) && (abs(X) == 1)) {
		Yaw = SpringArm->GetComponentRotation().Yaw + 90.f;
		Y = X;
		X = 0.f;
		directionTurnCamera = 1;
	}

	// Corner -+
	if ((!Corner->AxisX) && (Corner->AxisY) && (abs(Y) == 1)) {
		Yaw = SpringArm->GetComponentRotation().Yaw + 90.f;
		X = -Y;
		Y = 0.f;
		directionTurnCamera = 1;
	}
	else if ((!Corner->AxisX) && (Corner->AxisY) && (abs(X) == 1)) {
		Yaw = SpringArm->GetComponentRotation().Yaw - 90.f;
		Y = -X;
		X = 0.f;
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
		Teleport->StartingTeleport();
	}
}

void AMyPlayer::Shot()
{
	if (Bullet && bCanShot) {
		FRotator weaponDirection = GetMesh()->GetBoneQuaternion("weapon_l").Rotator();
		weaponDirection.Yaw += 90.f;
		FVector weaponLocation = GetMesh()->GetBoneLocation("weapon_l");
		FVector RotatedLocation = FQuat(weaponDirection).RotateVector(FVector(40, 0.f, 2.f));
		Barrel->SetWorldLocation(weaponLocation + RotatedLocation);
		
		FVector SpawnLocation = Barrel->GetComponentLocation();
		FRotator SpawnRotation = GetActorRotation();
		ABullet* newBullet = GetWorld()->SpawnActor<ABullet>(Bullet, SpawnLocation, SpawnRotation);
		newBullet->SetBulletParameters(weaponDirection.Vector(), this);
	}
}

void AMyPlayer::AutoAim(float dt)
{
	/*FVector StartLocation = GetActorLocation();
	FVector EndLocation = (StartLocation + GetActorForwardVector() * 1000.f) - FVector(70.f * Y, 70.f * X, 0.f);
	FRotator NewRotation;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);

	FHitResult HitResult;

	for (int i = 0; i <= 5; i++) {
		bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, CollisionParams);
		//DrawDebugLine(GetWorld(), StartLocation, HitResult.ImpactPoint, FColor::Green, false, 1, 0, 1);
		if (bHit)
		{
			if (HitResult.GetActor()->ActorHasTag("Enemy")) {
				//DrawDebugLine(GetWorld(), StartLocation, HitResult.ImpactPoint, FColor::Red, false, 1, 0, 1);

				NewRotation = UKismetMathLibrary::FindLookAtRotation(StartLocation, HitResult.GetActor()->GetActorLocation());
				NewRotation.Pitch = 0.f;
				float AbsNewRotationYaw = FMath::Abs(NewRotation.Yaw);
				//SetActorRotation(NewRotation);
				if (AbsNewRotationYaw <= (SaveRotation + 2.0f) && AbsNewRotationYaw >= (SaveRotation - 2.0f)) {
					bTurnRight = false;
					bTurnLeft = false;
				}
				if (bFirstSave) {
					bFirstSave = false;
					SaveRotation = AbsNewRotationYaw;
				}
				if (AbsNewRotationYaw >= (SaveRotation + 9.0f) || AbsNewRotationYaw <= (SaveRotation - 9.0f)) {
					if (AbsNewRotationYaw >= (SaveRotation + 9.0f))	bTurnRight = true;
					if (AbsNewRotationYaw <= (SaveRotation - 9.0f))	bTurnLeft = true;
					SaveRotation = AbsNewRotationYaw;
					SetActorRotation(NewRotation);
				}
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("NewRotation: %f %f %f %f"), NewRotation.Pitch, NewRotation.Roll, NewRotation.Yaw, SaveRotation));
				if (NewRotation.Yaw > 60.0f && NewRotation.Yaw < 120.0f) NewRotation.Yaw -= 90.0f;
				if (NewRotation.Yaw < -60.0f && NewRotation.Yaw > -120.0f) NewRotation.Yaw += 90.0f;
				if (NewRotation.Yaw < -160.0f && NewRotation.Yaw >= -180.0f)	NewRotation.Yaw += 180.0f;
				if (NewRotation.Yaw > 160.f && NewRotation.Yaw <= 180.0f)	NewRotation.Yaw -= 180.0f;
				//if (AbsNewRotationYaw > 60.0f && AbsNewRotationYaw <= 120.0f)	NewRotation.Yaw -= 90.0f;
				//if (AbsNewRotationYaw > 120.f && AbsNewRotationYaw <= 180.0f)	NewRotation.Yaw -= 180.0f;
				break;
			}
			bFirstSave = true;
		}
		EndLocation += FVector(50.f*Y, 50.f*X, 0.f);
	}
	CurrentEnemyRotation = FMath::RInterpTo(GetActorRotation(), NewRotation, dt, 5.0f);*/

	if (Enemy) {
		FVector StartLocation = GetActorLocation();
		FVector TargetEnemy = Enemy->TargetAim->GetComponentLocation() + FVector(0.f, 0.f, -70.f);
		ArmRotation = UKismetMathLibrary::FindLookAtRotation(StartLocation, TargetEnemy);
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("NewRotation: %f %f %f"), ArmRotation.Pitch, ArmRotation.Roll, ArmRotation.Yaw));
		float YawArmRotation = (ArmRotation.Yaw / 90.f);
		if (YawArmRotation >= 0.5f && YawArmRotation < 1.5f)	ArmRotation.Yaw -= 90.f;
		else if (YawArmRotation <= -0.5f && YawArmRotation > -1.5f)	ArmRotation.Yaw += 90.f;
		else if (YawArmRotation <= -1.5f || YawArmRotation > 1.5f)	ArmRotation.Yaw += int(YawArmRotation) * 180;
	}
	else {
		ArmRotation = FMath::RInterpTo(FRotator(0.f, 10.f, 0.f), ArmRotation, dt, 5.f);
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
			FRotator newRotation = FRotator(0.f, Yaw, 0.f);
			SpringArm->SetRelativeRotation(FQuat(newRotation), false, 0, ETeleportType::None);
			bTurnCamera = false;
		}
			
	}
}

void AMyPlayer::SwitchCanShot()
{
	bCanShot = !bCanShot;
	if(GetWorld()->GetName() != "Menu")	EnableInput(PlayerController);
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
	TargetLocation = TLocation;
	TargetLocation.Z = GetCapsuleComponent()->GetRelativeLocation().Z;
	bCorrect = true;
}

bool AMyPlayer::isDead()
{
	if (hp <= 0) return true;
	return false;
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
	AutoAim(DeltaTime);
	//DrawDebugBox(GetWorld(), ViewRange->GetComponentLocation(), ViewRange->GetScaledBoxExtent(), FQuat(GetActorRotation()), FColor::Turquoise, false, 0.5f, 0, 2);
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

void AMyPlayer::OnOverlapBox(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Enemy = Cast<AEnemy>(OtherActor);
}

void AMyPlayer::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (Cast<AEnemy>(OtherActor)) {
		Enemy = nullptr;
	}
}

