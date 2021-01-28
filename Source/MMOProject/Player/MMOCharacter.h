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

	//if this damage is represented by a condition, rather than damage to the character's health
	UFUNCTION(BlueprintCallable, Category = "Damage")
	static bool IsConditionDamage(struct FDamageEvent const& DamageEvent);

	//Called when the character dies
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat")
	void OnDeath();

	//Add threat to the threat list, with a specified threat level
	//Threat level indicates how much of a target priority this actor is to the other actor
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void AddThreat(AActor* InThreat, float InThreatLevel, bool bAffectOther = false);

	//Removes the specified threat from the threat list, bAffectOther controls whether we should remove ourself from the Threats threatlist as well
	//false by default to prevent accidental infinite loops
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void RemoveThreat(AActor* InThreat, bool bAffectOther = false);

	//Get the target that has the highest threat level
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Combat")
	AActor* GetHighestThreatTarget();

	//Clear all threats from this character
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void RemoveAllThreats();

	//Update the nameplate to face the camera
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

	//Get this characters relationship to the specified other actor
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Faction")
	EFactionRelationStatus GetFactionRelation(AActor* OtherActor);

	UFUNCTION()
	void OnRep_FactionRelationMap();

	//Get the Character's current health as a percentage of their max health
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Status")
	float GetHealthPercentage();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	float MaxHealth = 100.0f;

	//Set to true when the player presses left click
	UPROPERTY(Transient)
	bool bWantsToControlCamera = false;

	//Set to true when the player presses right click
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


	//Initial spawning of the player's nameplate
	UFUNCTION()
	void SpawnNameplate();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Camera")
	float MaxCameraZoomDistance = -2000.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Camera")
	float MinCameraZoomDistance = -100;

	//The character's current target actor
	UPROPERTY(ReplicatedUsing=OnRep_TargetActor, BlueprintReadOnly, Transient, Category = "Action")
	AActor* TargetActor;

	//the characters's current health
	UPROPERTY(ReplicatedUsing=OnRep_Health, BlueprintReadWrite, Transient, Category = "Status")
	float Health = 100.0f;

	//returns if this player is dead
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Status")
	virtual bool IsDead();

	//Set to true when the character enters combat (aggros an enemy or attacks an enemy)
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

	//Mana is not implemented yet
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


	//Set the characters target actor to the specified actor
	UFUNCTION(BlueprintCallable, Category = "Action")
	void SetTargetActor(AActor* InTargetActor);

	UFUNCTION(Server, Reliable, WithValidation, Category = "Action")
	void Server_SetTargetActor(AActor* InTargetActor);

	//The list of possible actions able to be taken by this character
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action")
	TArray<FName> ActionList;

	//The list of current combatants to this character
	UPROPERTY(EditAnywhere, ReplicatedUsing=OnRep_Threats, BlueprintReadWrite, Category = "Action")
	TArray<AActor*> Threats;

	//The map of the current combatants to this character, to that character's threat against this character
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action")
	TMap<AActor*, float> ThreatMap;

	UFUNCTION(BlueprintCallable)
	void OnRep_Threats();

	//Called when the character's threats are changed
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Combat")
	void OnThreatsChanged();

	//Returns whether or not this character is in combat
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Combat")
	bool IsInCombat();

};
