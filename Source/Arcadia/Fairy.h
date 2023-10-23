// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Fairy.generated.h"

UCLASS()
class ARCADIA_API AFairy : public APawn
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Fairy Settings")
	class UStaticMeshComponent* FairyMesh;

public:
	// Sets default values for this pawn's properties
	AFairy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
