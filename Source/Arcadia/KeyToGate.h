// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "KeyToGate.generated.h"

UCLASS()
class ARCADIA_API AKeyToGate : public AActor
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category = "Key Settings")
	class UStaticMeshComponent* KeyMesh;
	UPROPERTY(EditAnywhere, Category = "Key Settings")
	class USphereComponent* SphereCollider;

public:	
	// Sets default values for this actor's properties
	AKeyToGate();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	class AArcadiaGameModeBase* GameMode;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION()
	void OnOverlapBox(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
