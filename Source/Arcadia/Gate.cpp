// Fill out your copyright notice in the Description page of Project Settings.


#include "Gate.h"
#include "Components/BoxComponent.h"
#include "ArcadiaGameModeBase.h"

// Sets default values
AGate::AGate()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GateMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GateMesh"));
	RootComponent = GateMesh;

	BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollider"));
	BoxCollider->InitBoxExtent(FVector(50.f, 50.f, 50.f));
	BoxCollider->SetCollisionProfileName("Trigger");
	BoxCollider->SetupAttachment(GateMesh);

}

// Called when the game starts or when spawned
void AGate::BeginPlay()
{
	Super::BeginPlay();
	BoxCollider->OnComponentBeginOverlap.AddDynamic(this, &AGate::OnOverlapBox);
	BoxCollider->OnComponentEndOverlap.AddDynamic(this, &AGate::OnOverlapEnd);
	DrawDebugBox(GetWorld(), BoxCollider->GetComponentLocation(), BoxCollider->GetScaledBoxExtent(), FQuat(GetActorRotation()), FColor::Turquoise, true, -1, 0, 2);
	GameMode = GetWorld()->GetAuthGameMode<AArcadiaGameModeBase>();
	OpenGate();
}

void AGate::OpenGate()
{
	if (GameMode) {
		if(GameMode->openGate)
			GateMesh->SetMaterial(0, OpenGateM);
	}
}

// Called every frame
void AGate::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGate::OnOverlapBox(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AMyPlayer* player = Cast<AMyPlayer>(OtherActor)) {
		if (GameMode->openGate) {
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("You Win"));
		}
	}
}

void AGate::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (AMyPlayer* player = Cast<AMyPlayer>(OtherActor)) {
		
	}
}
