// Fill out your copyright notice in the Description page of Project Settings.


#include "Obstacle.h"
#include "MyPlayer.h"
#include "Components/BoxComponent.h"

// Sets default values
AObstacle::AObstacle()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ObstacleMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ObstacleMesh"));
	RootComponent = ObstacleMesh;

	BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollider"));
	BoxCollider->InitBoxExtent(FVector(50.f, 50.f, 50.f));
	BoxCollider->SetCollisionProfileName("Trigger");
	BoxCollider->SetupAttachment(ObstacleMesh);

	LavaCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("LavaCollider"));
	LavaCollider->InitBoxExtent(FVector(50.f, 50.f, 50.f));
	LavaCollider->SetupAttachment(ObstacleMesh);

}

// Called when the game starts or when spawned
void AObstacle::BeginPlay()
{
	Super::BeginPlay();
	
	BoxCollider->OnComponentBeginOverlap.AddDynamic(this, &AObstacle::OnOverlapBox);
}

// Called every frame
void AObstacle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AObstacle::OnOverlapBox(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AMyPlayer* player = Cast<AMyPlayer>(OtherActor)) {
		player->GetDmg(5);
	}
}

