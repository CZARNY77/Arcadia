// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Teleports.generated.h"

UCLASS()
class ARCADIA_API ATeleports : public AActor
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category = "Teleport settings")
	class USceneComponent* DummyRoot;

	UPROPERTY(EditAnywhere, Category = "Teleport settings")
	class UStaticMeshComponent* MeshTeleport_1;
	UPROPERTY(EditAnywhere, Category = "Teleport settings")
	class UBoxComponent* BoxCollider_1;

	UPROPERTY(EditAnywhere, Category = "Teleport settings")
	class UStaticMeshComponent* MeshTeleport_2;
	UPROPERTY(EditAnywhere, Category = "Teleport settings")
	class UBoxComponent* BoxCollider_2;
	UPROPERTY(EditAnywhere, Category = "Teleport settings")
	class UParticleSystem* TeleportEffect;

public:	
	// Sets default values for this actor's properties
	ATeleports();

	void StartingTeleport();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	class AMyPlayer* player;
	int teleportNumber;

	void TeleportPlayer();
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnOverlapBox(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
