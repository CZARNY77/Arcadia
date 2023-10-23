// Fill out your copyright notice in the Description page of Project Settings.


#include "Fairy.h"

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
	
}

// Called every frame
void AFairy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AFairy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

