// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bullet.generated.h"

UCLASS()
class ARCADIA_API ABullet : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Bullet Settings")
	class USphereComponent* Bullet;
	UPROPERTY(EditAnywhere, Category = "Bullet Settings")
	class UStaticMeshComponent* MeshBullet;
	UPROPERTY(EditAnywhere, Category = "Bullet Settings")
	class UProjectileMovementComponent* BulletMovementComponent;
	UPROPERTY(EditAnywhere, Category = "Bullet Settings")
	class UParticleSystem* DestroyParticleEmitter;
	UPROPERTY(EditAnywhere, Category = "Bullet Settings")
	class USphereComponent* CollisionComponent;
public:	
	// Sets default values for this actor's properties
	ABullet();

	void SetBulletVelocity(FVector bulletDirection);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void DestroyBullet();

	UPROPERTY(EditAnywhere, Category = "Bullet Settings")
	float speed;

	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
