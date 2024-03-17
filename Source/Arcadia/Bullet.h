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
public:	
	// Sets default values for this actor's properties
	ABullet();
	void SetBulletParameters(FVector bulletDirection, class AActor* player);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UFUNCTION(BlueprintCallable, Category = "Bullet class")
	void DestroyBullet();

	UPROPERTY(EditAnywhere, Category = "Bullet Settings")
	float speed;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
