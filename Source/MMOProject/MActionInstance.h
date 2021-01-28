// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MMOTypes.h"
#include "MActionInstance.generated.h"

UCLASS()
class MMOPROJECT_API AMActionInstance : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMActionInstance();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(Replicated, Transient, BlueprintReadWrite, meta = (ExposeOnSpawn = "true"), Category = "Action")
	FCharacterActionNotify ActionNotify;

	UPROPERTY(Replicated, Transient, BlueprintReadWrite, Category = "Action")
	bool bAppliedEffects;

};
