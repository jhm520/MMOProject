// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine.h"
#include "Engine/EngineTypes.h"
#include "MConditionDamage.h"
#include "UnrealNetwork.h"
#include "MMOTypes.generated.h"

UENUM(BlueprintType)
enum class EActionTargetType : uint8
{
	NoTarget, //No target required
	Target,	  //Target anything
	TargetSelf,	//Target self only
	TargetEnemy,	//Target enemies only
	TargetAlly,		//Target ally or self if no target selected
	GroundTarget
};

UENUM(BlueprintType)
enum class EActionCastType : uint8
{
	Instant, //Fires once immedately
	Cast, //Fires after cast time
	Channel //Fires immediately then produces effects continuously
};

UENUM(BlueprintType)
enum class EActionSpellType : uint8
{
	Instant, //Applies to target immediately
	Projectile //Fires then applies conditions when the projectile reaches the taret
};

UENUM(BlueprintType)
enum class EActionUseType : uint8
{
	Shot, //Fires once then cooldown
	Toggle, //Turns on/off
	AutoCast //Cast once then automatically casts after cooldown is over
};

UENUM(BlueprintType)
enum class EActionFailureType : uint8
{
	Cooldown, //Action was on cooldown
	NoTarget, //No target, and we required a target
	InvalidTarget, //Target was not valid for this action
	OutOfRange, //Too far away
	LineOfSight, //Can't see the target
	NotInFront, //Target wasn't in front of the player
	LackPermission, //You don't have permission to do that action
	Disabled, //You are stunned or prevented from casting
	IsCasting, //You are already casting
	TargetEvading //Target was evading
};

UENUM(BlueprintType)
enum class EFactionRelationStatus : uint8
{
	Enemy,	//Will aggro if within range, cannot be healed
	Neutral, //Will attack if attacked, will not come to aid, can be healed
	Ally,	//Cannot attack this target, will come to aid, can be healed
	Passive //Non-interactable
};

USTRUCT(blueprintType)
struct FActionStruct : public FTableRowBase 
{
	GENERATED_BODY()
public:

	UPROPERTY(blueprintReadWrite, editDefaultsOnly, category="Action")
	FName ActionName;

	UPROPERTY(blueprintReadWrite, editDefaultsOnly, category="Action")
	FText ActionSymbol;

	UPROPERTY(blueprintReadWrite, editDefaultsOnly, category="Action")
	FText DisplayName;

	UPROPERTY(blueprintReadWrite, editDefaultsOnly, category="Action")
	EActionTargetType TargetType;

	UPROPERTY(blueprintReadWrite, editDefaultsOnly, category="Action")
	EActionCastType CastType;

	UPROPERTY(blueprintReadWrite, editDefaultsOnly, category="Action")
	EActionSpellType SpellType;

	UPROPERTY(blueprintReadWrite, editDefaultsOnly, category="Action")
	float ProjectileSpeed = 1000.0f;

	UPROPERTY(blueprintReadWrite, editDefaultsOnly, category = "Action")
	float CastTime = 0.0f;

	UPROPERTY(blueprintReadWrite, editDefaultsOnly, category="Action")
	EActionUseType UseType;

	UPROPERTY(blueprintReadWrite, editDefaultsOnly, category = "Action")
	float damage;

	UPROPERTY(blueprintReadWrite, editDefaultsOnly, category = "Action")
	float CooldownTime = 1.5f;

	//Maximum range you can cast this spell at a target, 0 == unlimited
	UPROPERTY(blueprintReadWrite, editDefaultsOnly, category = "Action")
	float CastMaxRange = 0.0f;

	UPROPERTY(blueprintReadWrite, editDefaultsOnly, category = "Action")
	TSubclassOf<UDamageType> DamageType;

	//should this action do aggro to enemies
	UPROPERTY(blueprintReadWrite, editDefaultsOnly, category = "Action")
	bool bAggro = true;

	//should this action do aggro to enemies
	UPROPERTY(blueprintReadWrite, editDefaultsOnly, category = "Action")
	float AggroLevel = 1.0f;

};

USTRUCT(blueprintType)
struct FCharacterActionNotify
{
	GENERATED_BODY()
public:

	UPROPERTY(blueprintReadWrite, editDefaultsOnly, category = "Action")
	FActionStruct Action;

	UPROPERTY(blueprintReadWrite, editDefaultsOnly, category = "Action")
	AActor* Instigator;

	UPROPERTY(blueprintReadWrite, editDefaultsOnly, category = "Action")
	AActor* TargetActor;

	UPROPERTY(blueprintReadWrite, editDefaultsOnly, category = "Action")
	FVector TargetLocation;

	UPROPERTY(blueprintReadWrite, editDefaultsOnly, category = "Action")
	float TimeOfCast;

	UPROPERTY(blueprintReadWrite, editDefaultsOnly, category = "Action")
	bool bStopSignal;

	FCharacterActionNotify() {}

};