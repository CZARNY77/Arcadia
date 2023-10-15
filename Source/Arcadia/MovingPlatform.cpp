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
}

// Called when the game starts or when spawned
void AMovingPlatform::BeginPlay()
{
	Super::BeginPlay();

	DrawDebugBox(GetWorld(), Start->GetComponentLocation(), Start->GetScaledBoxExtent(), FQuat(GetActorRotation()), FColor::Turquoise, true, -1, 0, 2);
	DrawDebugBox(GetWorld(), End->GetComponentLocation(), End->GetScaledBoxExtent(), FQuat(GetActorRotation()), FColor::Turquoise, true, -1, 0, 2);
}

void AMovingPlatform::Moving(float dt)
{
	FVector VDirection = (Start->GetComponentLocation() - End->GetComponentLocation()) / 2;
	FVector MoveDelta = (VDirection * (FMath::Sin(GetWorld()->GetTimeSeconds() + dt) - FMath::Sin(GetWorld()->GetTimeSeconds())));

	PlatformMesh->AddLocalOffset(MoveDelta, true);
}

// Called every frame
void AMovingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Moving(DeltaTime);
}

