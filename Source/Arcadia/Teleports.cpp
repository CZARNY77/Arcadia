// Fill out your copyright notice in the Description page of Project Settings.


#include "Teleports.h"
#include "MyPlayer.h"
#include "Components/BoxComponent.h"
#include "ArcadiaGameModeBase.h"
#include "MyHUD.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ATeleports::ATeleports()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DummyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DummyRoot"));
	RootComponent = DummyRoot;

	MeshTeleport_1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshT_1"));
	MeshTeleport_1->SetupAttachment(RootComponent);

	MeshTeleport_2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshT_2"));
	MeshTeleport_2->SetupAttachment(RootComponent);

	BoxCollider_1 = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollider_1"));
	BoxCollider_1->InitBoxExtent(FVector(50.f, 50.f, 50.f));
	BoxCollider_1->SetCollisionProfileName("Trigger");
	BoxCollider_1->SetupAttachment(MeshTeleport_1);

	BoxCollider_2 = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollider_2"));
	BoxCollider_2->InitBoxExtent(FVector(50.f, 50.f, 50.f));
	BoxCollider_2->SetCollisionProfileName("Trigger");
	BoxCollider_2->SetupAttachment(MeshTeleport_2);

	teleportNumber = 0;
}

// Called when the game starts or when spawned
void ATeleports::BeginPlay()
{
	Super::BeginPlay();
	
	BoxCollider_1->OnComponentBeginOverlap.AddDynamic(this, &ATeleports::OnOverlapBox);
	BoxCollider_2->OnComponentBeginOverlap.AddDynamic(this, &ATeleports::OnOverlapBox);

	BoxCollider_1->OnComponentEndOverlap.AddDynamic(this, &ATeleports::OnOverlapEnd);
	BoxCollider_2->OnComponentEndOverlap.AddDynamic(this, &ATeleports::OnOverlapEnd);

	DrawDebugBox(GetWorld(), BoxCollider_1->GetComponentLocation(), BoxCollider_1->GetScaledBoxExtent(), FQuat(GetActorRotation()), FColor::Turquoise, true, -1, 0, 2);
	DrawDebugBox(GetWorld(), BoxCollider_2->GetComponentLocation(), BoxCollider_2->GetScaledBoxExtent(), FQuat(GetActorRotation()), FColor::Turquoise, true, -1, 0, 2);

	MyGameMode = Cast<AArcadiaGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
}

// Called every frame
void ATeleports::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATeleports::StartingTeleport()
{
	if (TeleportEffect) {
		FTimerHandle UnusedHandle;
		GetWorld()->GetTimerManager().SetTimer(UnusedHandle, this, &ATeleports::TeleportPlayer, 0.1f, false);
		if (teleportNumber == 1)
			UGameplayStatics::SpawnEmitterAtLocation(this, TeleportEffect, BoxCollider_1->GetComponentLocation(), GetActorRotation());
		else if (teleportNumber == 2)
			UGameplayStatics::SpawnEmitterAtLocation(this, TeleportEffect, BoxCollider_2->GetComponentLocation(), GetActorRotation());
	}
}

void ATeleports::TeleportPlayer()
{
	if (player) {
		if (teleportNumber == 1) {
			player->SetActorLocation(BoxCollider_2->GetComponentLocation());
			UGameplayStatics::SpawnEmitterAtLocation(this, TeleportEffect, BoxCollider_2->GetComponentLocation(), GetActorRotation());
		}
		else if (teleportNumber == 2) {
			player->SetActorLocation(BoxCollider_1->GetComponentLocation());
			UGameplayStatics::SpawnEmitterAtLocation(this, TeleportEffect, BoxCollider_1->GetComponentLocation(), GetActorRotation());
		}
	}
}


void ATeleports::OnOverlapBox(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Cast<AMyPlayer>(OtherActor)) {
		player = Cast<AMyPlayer>(OtherActor);
		if(player)	
			player->Teleport = this;

		teleportNumber = *OverlappedComp->GetName() == BoxCollider_1->GetName() ? 1 : 2;
		if (MyGameMode->MyHUD)	MyGameMode->MyHUD->DisplayActionButton();
	}
}

void ATeleports::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (Cast<AMyPlayer>(OtherActor) && player) {
		player->Teleport = nullptr;
		if (MyGameMode->MyHUD)	MyGameMode->MyHUD->HideActionButton();
	}
}

