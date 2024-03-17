// Fill out your copyright notice in the Description page of Project Settings.


#include "MovingPlatform.h"
#include "Components/BoxComponent.h"

// Sets default values
AMovingPlatform::AMovingPlatform()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DummyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DummyRoot"));
	RootComponent = DummyRoot;

	Start = CreateDefaultSubobject<UBoxComponent>(TEXT("Start"));
	Start->InitBoxExtent(FVector(50, 50, 10));
	Start->SetupAttachment(DummyRoot);

	End = CreateDefaultSubobject<UBoxComponent>(TEXT("End"));
	End->InitBoxExtent(FVector(50, 50, 10));
	End->SetupAttachment(DummyRoot);

	PlatformMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlatformMesh"));
	PlatformMesh->SetupAttachment(DummyRoot);

	speed = 100.f;
	ToStart = false;
	ToEnd = true;
}

// Called when the game starts or when spawned
void AMovingPlatform::BeginPlay()
{
	Super::BeginPlay();

	DrawDebugBox(GetWorld(), Start->GetComponentLocation(), Start->GetScaledBoxExtent(), FQuat(GetActorRotation()), FColor::Turquoise, true, -1, 0, 2);
	DrawDebugBox(GetWorld(), End->GetComponentLocation(), End->GetScaledBoxExtent(), FQuat(GetActorRotation()), FColor::Turquoise, true, -1, 0, 2);
	MovementDirection = (End->GetComponentLocation() - Start->GetComponentLocation()).GetSafeNormal();
}

void AMovingPlatform::Moving(float dt)
{
	FVector CurrentLocation = PlatformMesh->GetComponentLocation();
	double EndPoint = FVector::Dist(CurrentLocation, End->GetComponentLocation());
	double StartPoint = FVector::Dist(CurrentLocation, Start->GetComponentLocation());
	if ((EndPoint < 141.f && ToEnd) || (StartPoint < 141.f && ToStart)) {
		MovementDirection *= -1;
		ToStart = !ToStart;
		ToEnd = !ToEnd;
	}
	FVector AddLocation = MovementDirection * speed * dt;
	PlatformMesh->AddLocalOffset(AddLocation);
}

// Called every frame
void AMovingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Moving(DeltaTime);
}

