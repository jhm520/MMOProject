// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MShrine.generated.h"

UCLASS()
class MMOPROJECT_API AMShrine : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMShrine();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Shrine")
	void ReleaseSpirit(APlayerController* Controller, class AMMOCharacter* Character);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
