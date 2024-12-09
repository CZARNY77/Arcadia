// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Gate.generated.h"

UCLASS()
class ARCADIA_API AGate : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Gate Settings")
	class UStaticMeshComponent* GateMesh;
	UPROPERTY(EditAnywhere, Category = "Gate Settings")
	class UBoxComponent* BoxCollider;
	UPROPERTY(EditAnywhere, Category = "Gate Settings")
	class UMaterial* OpenGateM;
	UPROPERTY(EditAnywhere, Category = "Gate Settings")
	class UMaterial* CloseGateM;
	UPROPERTY(EditAnywhere, Category = "Gate Settings")
	class USceneComponent* Exit;
	UPROPERTY(EditAnywhere, Category = "Gate Settings")
	FName NextLevel;
	
public:	
	// Sets default values for this actor's properties
	AGate();

	void OpenGate();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	class AArcadiaGameModeBase* GameMode;

	void LoadNextLevel();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnOverlapBox(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UFUNCTION(BlueprintImplementableEvent, Category = "Gate Settings")
	void AddLevel();
};
