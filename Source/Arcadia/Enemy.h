// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"
#include "Animation/AnimMontage.h"
#include "Enemy.generated.h"

UCLASS()
class ARCADIA_API AEnemy : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Enemy Settings")
	class APathPatrol* PathPatrol;
	UPROPERTY(EditAnywhere, Category = "Enemy Settings")
	class UBoxComponent* ViewRange;
	UPROPERTY(EditAnywhere, Category = "Enemy Settings")
	class UAnimMontage* DeathMontage;
	UPROPERTY(EditAnywhere, Category = "Enemy Settings")
	class UAnimMontage* GetDmgMontage;
	UPROPERTY(EditAnywhere, Category = "Enemy Settings")
	class UParticleSystem* DestroyParticleEmitter;

	class AMyPlayer* player;
public:
	// Sets default values for this character's properties
	AEnemy();

	UPROPERTY(EditAnywhere, Category = "Enemy class")
	class USceneComponent* TargetAim;
	UFUNCTION(BlueprintCallable, Category = "Enemy class")
	void GetDmg(int dmg);
	UFUNCTION(BlueprintCallable, Category = "Enemy class")
	void Death();
	UPROPERTY(EditAnywhere, Category = "Shot Settings")
	class USceneComponent* Barrel;
	UPROPERTY(EditAnywhere, Category = "Shot Settings")
	TSubclassOf<class ABullet> Bullet;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shot Settings")
	bool bCanShot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shot Settings")
	FRotator ArmRotation;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	bool bPatrol;
	bool bIsDead;
	void Patroling(float dt);
	void FocusOnPlayer(float dt);
	void AutoAim(float dt);
	void Shot();

	int countSpline;
	int maxCountSpline;
	int tempCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Settings")
	int hp;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnOverlapBox(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UFUNCTION(BlueprintImplementableEvent, Category = "Shot Settings")
	void BPShot();
};
