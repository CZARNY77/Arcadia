// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bullet.generated.h"

UCLASS()
class ARCADIA_API ABullet : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "MeshBlock")
	class USceneComponent* DummyRoot;
	UPROPERTY(EditAnywhere, Category = "CollisionBullet")
	class USphereComponent* Bullet;
	UPROPERTY(EditAnywhere, Category = "MeshBullet")
	class UStaticMeshComponent* MeshBullet;

public:	
	// Sets default values for this actor's properties
	ABullet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
