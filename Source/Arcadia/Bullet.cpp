// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"

// Sets default values
ABullet::ABullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Bullet = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionBullet"));
	Bullet->InitSphereRadius(5.f);
	Bullet->SetCollisionProfileName("BlockAll");
	Bullet->SetCollisionObjectType(ECC_GameTraceChannel1);
	Bullet->IgnoreActorWhenMoving(this, true);
	RootComponent = Bullet;

	MeshBullet = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshBullet"));
	MeshBullet->SetupAttachment(Bullet);

	BulletMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("BulletMovement"));
	BulletMovementComponent->InitialSpeed = 1000.f;
	BulletMovementComponent->MaxSpeed = 1000.f;
	BulletMovementComponent->ProjectileGravityScale = 0.f;
	BulletMovementComponent->SetUpdatedComponent(Bullet);

	Bullet->OnComponentHit.AddDynamic(this, &ABullet::OnHit);

	speed = 2000.f;
}

void ABullet::SetBulletVelocity(FVector bulletDirection)
{
	BulletMovementComponent->Velocity = bulletDirection * speed;
}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();

	float TimeToDestroy = 3.f;
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ABullet::DestroyBullet, TimeToDestroy, false);
}

void ABullet::DestroyBullet()
{
	if (DestroyParticleEmitter) {

		UGameplayStatics::SpawnEmitterAtLocation(this, DestroyParticleEmitter, GetActorLocation(), GetActorRotation());
		if (IsValidLowLevel()) // sprawdza czy np. wskaŸnik na obiekt zosta³ ju¿ zwolniony lub wskazuje na nieprawid³ow¹ lokalizacjê w pamiêci
		{
			this->Destroy();
		}
	}
	else {
		if (IsValidLowLevel()) // sprawdza czy np. wskaŸnik na obiekt zosta³ ju¿ zwolniony lub wskazuje na nieprawid³ow¹ lokalizacjê w pamiêci
		{
			this->Destroy();
		}
	}
}

void ABullet::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor && OtherActor != this)
	{
		DestroyBullet();
	}
	DestroyBullet();
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

