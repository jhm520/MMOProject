// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/MMOCharacter.h"
#include "MMOMob.generated.h"

UCLASS()
class MMOPROJECT_API AMMOMob : public AMMOCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMMOMob(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void OnTargeted_Implementation(APlayerController* EventInstigator, AActor* TargetedBy) override;

	virtual void OnStoppedTargeting_Implementation(APlayerController* EventInstigator, AActor* TargetedBy) override;

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;


	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
	float AggroRadius = 500.0f;

	UPROPERTY(BlueprintReadWrite, Category = "AI")
	bool bAggroReset = false;

	UPROPERTY(Replicated, BlueprintReadWrite, Transient, Category = "Status")
	bool bIsEvading = false;
};
