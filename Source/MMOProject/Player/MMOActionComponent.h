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

	UFUNCTION(BlueprintCallable, client, reliable)
	virtual void ClientOnActionFailure(const FActionStruct& InActionStruct, EActionFailureType FailureReason);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Action")
	void OnActionFailure(const FActionStruct& InActionStruct, EActionFailureType FailureReason, bool bNotifyServer = true);

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, Category = "Action")
	void MulticastOnActionSuccess(const FCharacterActionNotify& InActionNotify);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Action")
	void OnActionSuccess(const FCharacterActionNotify& InActionNotify);

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


	UPROPERTY(EditDefaultsOnly,	BlueprintReadWrite, Category = "Action")
	TMap<FName, FActionStruct> ActionMap;

};
