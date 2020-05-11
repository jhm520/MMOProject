// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MMOTypes.h"
#include "MMOActionComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), blueprintable, blueprintType )
class MMOPROJECT_API UMMOActionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMMOActionComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	UFUNCTION(blueprintNativeEvent, BlueprintCallable, category="Action")
	void RequestDoAction(FName InActionName, AActor* TargetActor, FVector TargetLocation);

	UFUNCTION(server, reliable, withValidation)
	virtual void ServerRequestDoAction(FName InActionName, AActor* TargetActor, FVector TargetLocation);

	UFUNCTION(BlueprintCallable, server, reliable, withValidation)
	virtual void ClientOnActionFailure(const FActionStruct& InActionStruct, EActionFailureType FailureReason);

	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	void OnActionFailure(const FActionStruct& InActionStruct, EActionFailureType FailureReason);

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
