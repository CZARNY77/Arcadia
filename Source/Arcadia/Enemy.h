// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"

UCLASS()
class ARCADIA_API AEnemy : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Enemy Settings")
	class APathPatrol* PathPatrol;

	UPROPERTY(EditAnywhere, Category = "Enemy Settings")
	class UBoxComponent* ViewRange;


	class AMyPlayer* player;
public:
	// Sets default values for this character's properties
	AEnemy();

	void Death();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	bool bPatrol;
	void Patroling(float dt);
	void FocusOnPlayer(float dt);

	int countSpline;
	int maxCountSpline;
	int tempCount;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnOverlapBox(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
