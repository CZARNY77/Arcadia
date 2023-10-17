// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "PathPatrol.h"
#include "MyPlayer.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ViewRange = CreateDefaultSubobject<UBoxComponent>(TEXT("ViewRange"));
	ViewRange->InitBoxExtent(FVector(50, 100, 50));
	ViewRange->SetCollisionProfileName("Trigger");
	ViewRange->SetupAttachment(RootComponent);

	GetCharacterMovement()->bOrientRotationToMovement = false;

	countSpline = 0;
	tempCount = 1;
	bPatrol = true;
}

void AEnemy::Death()
{
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
	if (bPatrol && PathPatrol) {
		
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
	if (!bPatrol && player) {
		FVector TargetLocation = player->GetActorLocation();
		FVector Direction = (TargetLocation - GetActorLocation()).GetSafeNormal();
		AddMovementInput(Direction);
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
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Widze cie"));
		player = Cast<AMyPlayer>(OtherActor);
		bPatrol = false;
	}

}

void AEnemy::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (Cast<AMyPlayer>(OtherActor)) {
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Uciekasz?"));
		bPatrol = true;
	}
}
