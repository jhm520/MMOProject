// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MMOClickableInterface.h"
#include "MMOCharacter.generated.h"

UCLASS()
class MMOPROJECT_API AMMOCharacter : public ACharacter, public IMMOClickableInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMMOCharacter(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void TickNameplate();

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void MoveForward(float InAxis);

	virtual void TurnRight(float InAxis);

	virtual void LookRight(float InAxis);

	virtual void LookUp(float InAxis);

	UFUNCTION()
	virtual void OnMeshClicked(UPrimitiveComponent* TouchedComponent, FKey ButtonPressed);

	virtual void OnControlCameraPressed();

	virtual void OnControlCameraReleased();

	virtual void OnControlPlayerPressed();

	virtual void OnControlPlayerReleased();

	virtual void OnCameraZoomIn();

	virtual void OnCameraZoomOut();

	virtual void OnJumpPressed();

	void OnLeftClicked_Implementation(APlayerController* EventInstigator);

	void OnRightClicked_Implementation(APlayerController* EventInstigator);

	void OnTargeted_Implementation(APlayerController* EventInstigator, AActor* TargetedBy);

	void OnStoppedTargeting_Implementation(APlayerController* EventInstigator, AActor* TargetedBy);

	FString GetFriendlyName_Implementation();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MMO")
	FString CharacterName = "";

	UPROPERTY(Transient)
	bool bWantsToControlCamera = false;

	UPROPERTY(Transient)
	bool bWantsToControlPlayer = false;

	UPROPERTY(Transient)
	FRotator CameraControlRotator = FRotator::ZeroRotator;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Camera")
	class USceneComponent* CameraRoot;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Camera")
	class UCameraComponent* PlayerCameraComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Camera")
	class UStaticMeshComponent* TargetingCircleMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	class UMMOWidgetComponent* NameplateWidgetComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TSubclassOf<class UMMOUserWidget> NameplateWidgetClass;

	UFUNCTION()
	void SpawnNameplate();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Camera")
	float MaxCameraZoomDistance = -2000.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Camera")
	float MinCameraZoomDistance = -100;

	UPROPERTY(ReplicatedUsing=OnRep_TargetActor, BlueprintReadOnly, Transient, Category = "Action")
	AActor* TargetActor;

	UPROPERTY(ReplicatedUsing=OnRep_Health, BlueprintReadWrite, Transient, Category = "Status")
	float Health = 100.0f;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Status")
	bool IsDead();

	UPROPERTY(ReplicatedUsing=OnRep_bIsInCombat, BlueprintReadWrite, Transient, Category = "Status")
	bool bIsInCombat = false;

	UFUNCTION()
	void OnRep_bIsInCombat();

	UFUNCTION()
	void OnRep_Health();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Status")
	void OnHealthChanged();

	UPROPERTY(ReplicatedUsing=OnRep_Mana, BlueprintReadWrite, Transient, Category = "Status")
	float Mana = 100.0f;

	UFUNCTION()
	void OnRep_Mana();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Status")
	void OnManaChanged();

	UFUNCTION()
	void OnRep_TargetActor();

	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	void OnTargetActorChanged();

	UFUNCTION(BlueprintCallable, Category = "Action")
	void SetTargetActor(AActor* InTargetActor);

	UFUNCTION(Server, Reliable, WithValidation, Category = "Action")
	void Server_SetTargetActor(AActor* InTargetActor);

};
