// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "PathPatrol.h"
#include "MyPlayer.h"
#include "Animation/AnimInstance.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ViewRange = CreateDefaultSubobject<UBoxComponent>(TEXT("ViewRange"));
	ViewRange->InitBoxExtent(FVector(50, 100, 50));
	ViewRange->SetCollisionProfileName("Trigger");
	ViewRange->SetupAttachment(RootComponent);

	TargetAim = CreateDefaultSubobject<USceneComponent>(TEXT("TargetAim"));
	TargetAim->SetupAttachment(RootComponent);

	Barrel = CreateDefaultSubobject<USceneComponent>(TEXT("Barrel"));
	Barrel->SetupAttachment(GetMesh());

	GetCharacterMovement()->bOrientRotationToMovement = false;

	countSpline = 0;
	tempCount = 1;
	bPatrol = true;
	bCanShot = true;
	bIsDead = false;
}

void AEnemy::GetDmg(int dmg)
{
	hp -= dmg;
	if (hp <= 0) {
		bIsDead = true;
		bPatrol = false;
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		if (DeathMontage) {
			PlayAnimMontage(DeathMontage);
			return;
		}
	}
	PlayAnimMontage(GetDmgMontage);
}

void AEnemy::Death()
{
	if (DestroyParticleEmitter) {
		UGameplayStatics::SpawnEmitterAtLocation(this, DestroyParticleEmitter, GetMesh()->GetComponentLocation(), GetActorRotation());
	}
	Destroy();
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	ViewRange->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::OnOverlapBox);
	ViewRange->OnComponentEndOverlap.AddDynamic(this, &AEnemy::OnOverlapEnd);

	if(PathPatrol)
		maxCountSpline = PathPatrol->SplineComponent->GetNumberOfSplinePoints();
}

void AEnemy::Patroling(float dt)
{
	if (bPatrol && PathPatrol && !bIsDead) {
		
		FVector TargetLocation = PathPatrol->SplineComponent->GetLocationAtSplinePoint(countSpline, ESplineCoordinateSpace::World);
		TargetLocation.Z = GetActorLocation().Z;
		FVector Direction = (TargetLocation - GetActorLocation()).GetSafeNormal();
		AddMovementInput(Direction);

		if ((FVector::Distance(GetActorLocation(), TargetLocation) < 10.f) && (countSpline < maxCountSpline) && (countSpline >= 0)) {
			countSpline += tempCount;
		}
		else if ((countSpline >= maxCountSpline) || (countSpline < 0)) {
			tempCount *= -1;
			countSpline += tempCount;
		}
	}
}

void AEnemy::FocusOnPlayer(float dt)
{

	if (!bPatrol && player && !bIsDead) {
		AutoAim(dt);
		Shot();
	}
}

void AEnemy::AutoAim(float dt)
{
	if (player) {
		FVector StartLocation = GetActorLocation();
		FVector TargetEnemy = player->TargetAim->GetComponentLocation() + FVector(0.f, 0.f, -70.f);
		ArmRotation = UKismetMathLibrary::FindLookAtRotation(StartLocation, TargetEnemy);
		float YawArmRotation = (ArmRotation.Yaw / 90.f);
		if (YawArmRotation >= 0.5f && YawArmRotation < 1.5f)	ArmRotation.Yaw -= 90.f;
		else if (YawArmRotation <= -0.5f && YawArmRotation > -1.5f)	ArmRotation.Yaw += 90.f;
		else if (YawArmRotation <= -1.5f || YawArmRotation > 1.5f)	ArmRotation.Yaw += int(YawArmRotation) * 180;
	}
	else {
		ArmRotation = FMath::RInterpTo(FRotator(0.f, 10.f, 0.f), ArmRotation, dt, 5.f);
	}
}

void AEnemy::Shot()
{
	if (Bullet && bCanShot && !player->isDead()) {
		FRotator weaponDirection = GetMesh()->GetBoneQuaternion("weapon_l").Rotator();
		weaponDirection.Yaw += 90.f;
		FVector weaponLocation = GetMesh()->GetBoneLocation("weapon_l");
		FVector RotatedLocation = FQuat(weaponDirection).RotateVector(FVector(40, 0.f, 2.f));
		Barrel->SetWorldLocation(weaponLocation + RotatedLocation);

		FVector SpawnLocation = Barrel->GetComponentLocation();
		FRotator SpawnRotation = GetActorRotation();
		ABullet* newBullet = GetWorld()->SpawnActor<ABullet>(Bullet, SpawnLocation, SpawnRotation);
		newBullet->SetBulletParameters(weaponDirection.Vector(), this);
		BPShot();
	}
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Patroling(DeltaTime);
	FocusOnPlayer(DeltaTime);
}

void AEnemy::OnOverlapBox(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Cast<AMyPlayer>(OtherActor)) {
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Widze cie"));
		player = Cast<AMyPlayer>(OtherActor);
		bPatrol = false;
	}

}

void AEnemy::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (Cast<AMyPlayer>(OtherActor)) {
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Uciekasz?"));
		bPatrol = true;
		player = nullptr;
	}
}
