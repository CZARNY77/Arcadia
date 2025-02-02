// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Corner.generated.h"


UCLASS()
class ARCADIA_API ACorner : public AActor
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category = "MeshBlock")
	class USceneComponent* DummyRoot;
	UPROPERTY(EditAnywhere, Category = "MeshBlock")
	class UBoxComponent* MeshBox;

public:	
	// Sets default values for this actor's properties
	ACorner();

	UPROPERTY(EditAnywhere, Category = "Corner Settings")
	bool AxisX;
	UPROPERTY(EditAnywhere, Category = "Corner Settings")
	bool AxisY;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	class AArcadiaGameModeBase* MyGameMode;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnOverlapBox(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
