// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MMOClickableInterface.h"
#include "MHUD.h"
#include "MMOTypes.h"
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

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat")
	void OnDeath();

	//Add threat to the threatlist
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void AddThreat(AActor* InThreat, bool bAffectOther = false);

	//Removes the specified threat from the threat list, bAffectOther controls whether we should remove ourself from the Threats threatlist as well
	//false by default to prevent accidental infinite loops
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void RemoveThreat(AActor* InThreat, bool bAffectOther = false);

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void RemoveAllThreats();

	void TickNameplate();

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void MoveForward(float InAxis);

	virtual void TurnRight(float InAxis);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
	float TurnRate = 90.0f;

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

	UPROPERTY(EditAnywhere, ReplicatedUsing=OnRep_FactionName, BlueprintReadWrite, Category = "MMO")
	FName FactionName = NAME_None;

	UFUNCTION()
	void OnRep_FactionName();

	UPROPERTY(EditAnywhere, ReplicatedUsing=OnRep_FactionRelationMap, BlueprintReadWrite, Category = "Faction")
	TMap<FName, EFactionRelationStatus> FactionRelationMap;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Faction")
	EFactionRelationStatus GetFactionRelation(AActor* OtherActor);

	UFUNCTION()
	void OnRep_FactionRelationMap();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Status")
	float GetHealthPercentage();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	float MaxHealth = 100.0f;

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
	virtual bool IsDead();

	UPROPERTY(ReplicatedUsing=OnRep_bIsInCombat, BlueprintReadWrite, Transient, Category = "Status")
	bool bIsInCombat = false;



	UFUNCTION()
	void OnRep_bIsInCombat();

	UFUNCTION()
	void OnRep_Health();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Status")
	void OnHealthChanged();

	//Update the local player's hud
	void UpdatePlayerHUD(EHudUpdateType UpdateType = EHudUpdateType::All);

	//if this character is targeted by someone, update that player's player hud
	void UpdateTargetHUD(EHudUpdateType UpdateType = EHudUpdateType::All);

	UPROPERTY(ReplicatedUsing=OnRep_Mana, BlueprintReadWrite, Transient, Category = "Status")
	float Mana = 100.0f;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Status")
	float GetManaPercentage();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	float MaxMana = 100.0f;

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action")
	TArray<FName> ActionList;

	//The list of current combatants to this character
	UPROPERTY(EditAnywhere, ReplicatedUsing=OnRep_Threats, BlueprintReadWrite, Category = "Action")
	TArray<AActor*> Threats;

	UFUNCTION(BlueprintCallable)
	void OnRep_Threats();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Combat")
	void OnThreatsChanged();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Combat")
	bool IsInCombat();

};
