// Fill out your copyright notice in the Description page of Project Settings.


#include "MMOMob.h"

// Sets default values
AMMOMob::AMMOMob(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMMOMob::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMMOMob::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMMOMob::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AMMOMob::OnTargeted_Implementation(APlayerController* EventInstigator, AActor* TargetedBy)
{
	Super::OnTargeted_Implementation(EventInstigator, TargetedBy);
}

void AMMOMob::OnStoppedTargeting_Implementation(APlayerController* EventInstigator, AActor* TargetedBy)
{
	Super::OnStoppedTargeting_Implementation(EventInstigator, TargetedBy);
}
