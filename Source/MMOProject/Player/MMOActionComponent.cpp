// Fill out your copyright notice in the Description page of Project Settings.


#include "MMOActionComponent.h"

// Sets default values for this component's properties
UMMOActionComponent::UMMOActionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);

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
	if(GetOwner()->GetLocalRole() < ROLE_Authority) {
		ServerRequestDoAction(InActionName, TargetActor, TargetLocation);
	}
	else
	{
		
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

void UMMOActionComponent::ClientOnActionFailure_Implementation(const FActionStruct& InActionStruct, EActionFailureType FailureReason)
{
	OnActionFailure(InActionStruct, FailureReason);
}

bool UMMOActionComponent::ClientOnActionFailure_Validate(const FActionStruct& InActionStruct, EActionFailureType FailureReason)
{
	return true;
}

void UMMOActionComponent::OnActionFailure_Implementation(const FActionStruct& InActionStruct, EActionFailureType FailureReason)
{

}


void UMMOActionComponent::MulticastOnActionSuccess_Implementation(const FCharacterActionNotify& InActionNotify)
{
	OnActionSuccess(InActionNotify);
}

void UMMOActionComponent::OnActionSuccess_Implementation(const FCharacterActionNotify& InActionStruct)
{

}

// Called every frame
void UMMOActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}
