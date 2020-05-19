// Fill out your copyright notice in the Description page of Project Settings.


#include "MShrine.h"

// Sets default values
AMShrine::AMShrine()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

}

// Called when the game starts or when spawned
void AMShrine::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMShrine::ReleaseSpirit_Implementation(APlayerController* Controller, class AMMOCharacter* Character)
{
	
}

// Called every frame
void AMShrine::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

