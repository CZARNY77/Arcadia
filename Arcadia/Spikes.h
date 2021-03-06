// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Spikes.generated.h"

UCLASS()
class ARCADIA_API ASpikes : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Components")
	class USceneComponent* DummyRoot;

	UPROPERTY(EditAnywhere, Category = "Components")
	class UBoxComponent* Box;

	UPROPERTY(EditAnywhere, Category = "Components")
	class UStaticMeshComponent* Mesh;
	
public:	
	// Sets default values for this actor's properties
	ASpikes();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
