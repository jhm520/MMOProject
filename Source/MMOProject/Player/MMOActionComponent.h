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
	//Client calls this to request from the server to do an action
	UFUNCTION(blueprintNativeEvent, BlueprintCallable, category="Action")
	void RequestDoAction(const FName& InActionName, AActor* TargetActor, FVector TargetLocation);

	//Server follows up and does the action
	UFUNCTION(BlueprintCallable, category="Action")
	void AuthDoAction(const FActionStruct& InActionStruct, AActor* TargetActor, FVector TargetLocation);

	//Server follows up and does the action
	UFUNCTION(BlueprintNativeEvent, category="Action")
	void ApplyActionNative(const FActionStruct& InActionStruct, AActor* TargetActor, FVector TargetLocation);

	//Server follows up and does the action
	UFUNCTION(BlueprintNativeEvent, category="Action")
	void StartCastingNative(const FActionStruct& InActionStruct, AActor* TargetActor, FVector TargetLocation);

	//Returns if this action is valid to call right now, given the Action, Target actor, and target location
	UFUNCTION(BlueprintCallable, BlueprintPure, category="Action")
	bool IsActionValidNative(const FActionStruct& InActionStruct, AActor* TargetActor, FVector TargetLocation, EActionFailureType& OutActionFailureType);

	//Returns if this action is within range for the supplied target actor
	UFUNCTION(BlueprintCallable, BlueprintPure, category="Action")
	bool IsActionInRangeNative(const FName& InActionName, AActor* TargetActor);

	//Returns if this action component is already casting a spell
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Action")
	bool IsCastingSpellNative();

	//the currently casting spell
	UPROPERTY(Replicated, Transient, BlueprintReadWrite, Category = "Casting")
	FName CastingSpell;

	//spells that are currently marked for auto casting
	UPROPERTY(Replicated, Transient, BlueprintReadWrite, Category = "Casting")
	TArray<FName> AutoCastList;

	//Returns if this action is on cooldown
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Action")
	bool IsActionOnCooldownNative(const FName& InActionName, AActor*& OutActionInstance);

	//Get the target actor of this component's owning MMO Character
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Target")
	AActor* GetTargetActorNative();

	//A map from Action FName to the Action Instance actor, how we store which spells are on cooldown
	UPROPERTY(Transient, BlueprintReadWrite, Category = "Action")
	TMap<FName, AActor*> ActionInstanceMap;

	//Server RPC for calling an action on the server
	UFUNCTION(server, reliable, withValidation)
	virtual void ServerRequestDoAction(const FName& InActionName, AActor* TargetActor, FVector TargetLocation);

	//Client RPC called when an action fails
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
