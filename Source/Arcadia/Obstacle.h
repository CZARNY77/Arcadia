// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Obstacle.generated.h"

UCLASS()
class ARCADIA_API AObstacle : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Obstacle Settings")
	class UStaticMeshComponent* ObstacleMesh;
	UPROPERTY(EditAnywhere, Category = "Obstacle Settings")
	class UBoxComponent* BoxCollider;
	
	UPROPERTY(EditAnywhere, Category = "Obstacle Settings")
	class UBoxComponent* LavaCollider;

public:	
	// Sets default values for this actor's properties
	AObstacle();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnOverlapBox(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
