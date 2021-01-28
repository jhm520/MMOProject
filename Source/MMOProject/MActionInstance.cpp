// Fill out your copyright notice in the Description page of Project Settings.


#include "MActionInstance.h"

// Sets default values
AMActionInstance::AMActionInstance()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMActionInstance::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMActionInstance::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMActionInstance::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMActionInstance, ActionNotify);
	DOREPLIFETIME(AMActionInstance, bAppliedEffects);

}

