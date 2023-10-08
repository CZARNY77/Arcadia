// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"
#include "Components/SphereComponent.h"

// Sets default values
ABullet::ABullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DummyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DummyRoot"));
	RootComponent = DummyRoot;

	Bullet = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionBullet"));
	Bullet->InitSphereRadius(5.f);
	Bullet->SetCollisionProfileName("Trigger");
	Bullet->SetupAttachment(DummyRoot);

	MeshBullet = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshBullet"));
	MeshBullet->SetupAttachment(Bullet);
}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

