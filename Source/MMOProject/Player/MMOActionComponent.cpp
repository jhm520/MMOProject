// Fill out your copyright notice in the Description page of Project Settings.


#include "MMOActionComponent.h"

// Sets default values for this component's properties
UMMOActionComponent::UMMOActionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	SetIsReplicated(true);

	// ...
}


// Called when the game starts
void UMMOActionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UMMOActionComponent::RequestDoAction_Implementation(FName InActionName, AActor* TargetActor, FVector TargetLocation)
{
	if(GetOwner()->Role < ROLE_Authority) {
		ServerRequestDoAction(InActionName, TargetActor, TargetLocation);
	}
	else {
		
	}
}

void UMMOActionComponent::ServerRequestDoAction_Implementation(FName InActionName, AActor* TargetActor, FVector TargetLocation)
{
	RequestDoAction(InActionName, TargetActor, TargetLocation);
}

bool UMMOActionComponent::ServerRequestDoAction_Validate(FName InActionName, AActor* TargetActor, FVector TargetLocation)
{
	return true;
}

// Called every frame
void UMMOActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

