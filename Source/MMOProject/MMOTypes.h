// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine.h"
#include "Engine/EngineTypes.h"
#include "MMOTypes.generated.h"

UENUM(BlueprintType)
enum class EActionTargetType : uint8
{
	NoTarget,
	Target,
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
	Disabled //You are stunned or prevented from casting
};

USTRUCT(blueprintType)
struct FActionStruct : public FTableRowBase
{
	GENERATED_BODY()
public:

	UPROPERTY(blueprintReadWrite, editDefaultsOnly, category="Action")
	FName ActionName;

	UPROPERTY(blueprintReadWrite, editDefaultsOnly, category="Action")
	EActionTargetType TargetType;

	UPROPERTY(blueprintReadWrite, editDefaultsOnly, category="Action")
	EActionCastType CastType;

	UPROPERTY(blueprintReadWrite, editDefaultsOnly, category = "Action")
	float CastTime = 0.0f;

	UPROPERTY(blueprintReadWrite, editDefaultsOnly, category="Action")
	EActionUseType UseType;

	UPROPERTY(blueprintReadWrite, editDefaultsOnly, category = "Action")
	float damage;

	UPROPERTY(blueprintReadWrite, editDefaultsOnly, category = "Action")
	float CooldownTime = 1.5f;

	UPROPERTY(blueprintReadWrite, editDefaultsOnly, category = "Action")
	TSubclassOf<UDamageType> DamageType;

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