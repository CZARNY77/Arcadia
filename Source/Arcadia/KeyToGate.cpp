// Fill out your copyright notice in the Description page of Project Settings.


#include "KeyToGate.h"
#include "Components/SphereComponent.h"
#include "ArcadiaGameModeBase.h"

// Sets default values
AKeyToGate::AKeyToGate()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	KeyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("KeyMesh"));
	RootComponent = KeyMesh;

	SphereCollider = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollider"));
	SphereCollider->InitSphereRadius(10.f);
	SphereCollider->SetCollisionProfileName("Trigger");
	SphereCollider->SetupAttachment(KeyMesh);

}

// Called when the game starts or when spawned
void AKeyToGate::BeginPlay()
{
	Super::BeginPlay();
	SphereCollider->OnComponentBeginOverlap.AddDynamic(this, &AKeyToGate::OnOverlapBox);
	GameMode = GetWorld()->GetAuthGameMode<AArcadiaGameModeBase>();
}

// Called every frame
void AKeyToGate::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AKeyToGate::OnOverlapBox(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Cast<AMyPlayer>(OtherActor)) {
		GameMode->PickUpKey();
		Destroy();
	}
}

