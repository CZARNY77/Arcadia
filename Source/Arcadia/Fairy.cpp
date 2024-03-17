// Fill out your copyright notice in the Description page of Project Settings.


#include "Fairy.h"
#include "ArcadiaGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "MyPlayer.h"

// Sets default values
AFairy::AFairy()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	FairyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FairyMesh"));
	RootComponent = FairyMesh;
}

// Called when the game starts or when spawned
void AFairy::BeginPlay()
{
	Super::BeginPlay();
	MyGameMode = Cast<AArcadiaGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	player = Cast<AMyPlayer>(MyGameMode->PlayerController->GetPawn());
    MyGameMode->MyFairy = this;
}

void AFairy::Fly(float dt)
{
    if (player) {
        FVector PlayerLocation = player->GetActorLocation();
        FVector NewLocation = CalculateFairyMovement(PlayerLocation);
        SetActorLocation(NewLocation);
    }
}


FVector AFairy::CalculateFairyMovement(const FVector& PlayerLocation)
{
    // Parametry ruchu
    float OrbitRadius = 100.0f;
    float Speed = 1.0f;

    // Aktualny czas od startu gry
    float Time = GetWorld()->GetTimeSeconds();

    // Obliczanie pozycji wró¿ki u¿ywaj¹c funkcji sinus i cosinus dla efektu orbitowania
    FVector Offset;
    Offset.X = FMath::Sin(Time * Speed) * OrbitRadius;
    Offset.Y = FMath::Cos(Time * Speed) * OrbitRadius;
    Offset.Z = FMath::Sin(Time * Speed * 0.5) * 50.0f; // Ma³a oscylacja pionowa dla dodania nieregularnoœci

    return PlayerLocation + Offset;
}

// Called every frame
void AFairy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Fly(DeltaTime);
}

// Called to bind functionality to input
void AFairy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

