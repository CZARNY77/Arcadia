// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MovingPlatform.generated.h"

UCLASS()
class ARCADIA_API AMovingPlatform : public AActor
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category = "Platform Settings")
	class USceneComponent* DummyRoot;
	UPROPERTY(EditAnywhere, Category = "Platform Settings")
	class UBoxComponent* Start;
	UPROPERTY(EditAnywhere, Category = "Platform Settings")
	class UBoxComponent* End;
	UPROPERTY(EditAnywhere, Category = "Platform Settings")
	class UStaticMeshComponent* PlatformMesh;

public:	
	// Sets default values for this actor's properties
	AMovingPlatform();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void Moving(float dt);

	UPROPERTY(EditAnywhere, Category = "Platform Settings")
	int speed;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
