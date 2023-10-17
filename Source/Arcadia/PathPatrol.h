// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SplineComponent.h"
#include "GameFramework/Actor.h"
#include "PathPatrol.generated.h"

UCLASS()
class ARCADIA_API APathPatrol : public AActor
{
	GENERATED_BODY()
public:	
	// Sets default values for this actor's properties
	APathPatrol();
	UPROPERTY(VisibleAnywhere)
	class USplineComponent* SplineComponent;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
