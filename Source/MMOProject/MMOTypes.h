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

UENUM(BlueprintType)
enum class EConditionDamageApplicationType : uint8
{
	None, //no damage
	Instant, //full damage applied immediately
	Delay, //full damage after a delay (unless dispelled)
	DOT, //damage over length of condition
	Incremental, //damage applied in increments
};

UENUM(BlueprintType)
enum class EConditionType : uint8
{
	None, //no condition
	Pure, //condition can't be cured
	Curse,
	Magic,
	Poison,
	Disease

};

UENUM(BlueprintType)
enum class EConditionEffectType : uint8
{
	None, //no effect
	Taunt, //causes target to focus on the attacker
};

UENUM(BlueprintType)
enum class EConditionDamageType : uint8
{
	None, //no damage
	Pure, //pure, raw, unscaled damage
	Fire,
	Water,
	Air,
	Earth,
	Light,
	Shadow,
	Bleed,
	Poison,
	Disease
};

USTRUCT(blueprintType)
struct FConditionEffectStruct
{
	GENERATED_BODY()
public:


	//the name of this component of the condition
	UPROPERTY(blueprintReadWrite, editDefaultsOnly, category = "Condition")
	FName ConditionEffectName = NAME_None;

	//What this condition effect does (other than damage)
	UPROPERTY(blueprintReadWrite, editDefaultsOnly, category = "Action")
	EConditionEffectType EffectType = EConditionEffectType::None;

	//How this condition's damage is applied
	UPROPERTY(blueprintReadWrite, editDefaultsOnly, category = "Effect")
	EConditionDamageApplicationType DamageApplicationType = EConditionDamageApplicationType::None;

	//What type of damage this condition effect does, by enum
	UPROPERTY(blueprintReadWrite, editDefaultsOnly, category = "Effect")
	EConditionDamageType DamageType = EConditionDamageType::None;

	//What damage class this condition effect applies
	UPROPERTY(blueprintReadWrite, editDefaultsOnly, category = "Action")
	TSubclassOf<UDamageType> DamageTypeClass;

	//Applies damage as soon as this condition effect is applied
	UPROPERTY(blueprintReadWrite, editDefaultsOnly, category = "Damage")
	float InstantDamage = 0.0f;

	//Applies damage over the course of this condition
	UPROPERTY(blueprintReadWrite, editDefaultsOnly, category = "Damage")
	float DamageOverTime = 0.0f;

	//How how much time between each increment, for both incremental and DOT damage
	UPROPERTY(blueprintReadWrite, editDefaultsOnly, category = "Damage")
	float TimeBetweenIncrement = 0.0f;

	//How long to wait before applying Delay damage
	UPROPERTY(blueprintReadWrite, editDefaultsOnly, category = "Damage")
	float DelayTime = 0.0f;

	//How much damage is applied after the delay
	UPROPERTY(blueprintReadWrite, editDefaultsOnly, category = "Damage")
	float DelayDamage = 0.0f;

	UPROPERTY(blueprintReadWrite, editDefaultsOnly, category = "Damage")
	int32 NumberOfIncrements = 0;

	//How much damage is applied per increment
	UPROPERTY(blueprintReadWrite, editDefaultsOnly, category = "Damage")
	float DamageIncrement = 0.0f;

};

USTRUCT(blueprintType)
struct FConditionStruct
{
	GENERATED_BODY()
public:

	UPROPERTY(blueprintReadWrite, editDefaultsOnly, category = "Condition")
	FName ConditionName = NAME_None;

	UPROPERTY(blueprintReadWrite, editDefaultsOnly, category = "Condition")
	FText DisplayName;

	//The effects of this condition
	UPROPERTY(blueprintReadWrite, editDefaultsOnly, category = "Condition")
	TArray<FConditionEffectStruct> ConditionEffects;

	//Condition lasts until dispelled or target dies
	UPROPERTY(blueprintReadWrite, editDefaultsOnly, category="Condition")
	bool bIndefinite = false;

	//How long this condition lasts
	UPROPERTY(blueprintReadWrite, editDefaultsOnly, category="Condition")
	float ConditionTimeLength = 15.0f;

	//What type of condition this is (how to dispell it)
	UPROPERTY(blueprintReadWrite, editDefaultsOnly, category="Condition")
	EConditionType ConditionType = EConditionType::None;
};

USTRUCT(blueprintType)
struct FConditionNotify
{
	GENERATED_BODY()
public:

	//The target of this condition
	UPROPERTY(blueprintReadWrite, editDefaultsOnly, category = "Condition")
	AActor* TargetActor = nullptr;

	//The actor responsible for applying this condition
	UPROPERTY(blueprintReadWrite, editDefaultsOnly, category = "Condition")
	AActor* Instigator = nullptr;

	//What time this condition was applied
	UPROPERTY(blueprintReadWrite, editDefaultsOnly, category = "Condition")
	float ConditionStartTime = 0.0f;

	//What time this condition was applied
	UPROPERTY(blueprintReadWrite, editDefaultsOnly, category = "Condition")
	FConditionStruct Condition = FConditionStruct();

	FConditionNotify() {}
};


USTRUCT(blueprintType)
struct FEquipmentStruct : public FTableRowBase
{
	GENERATED_BODY()
public:

	UPROPERTY(blueprintReadWrite, editDefaultsOnly, category = "Action")
	FName EquipmentName;

	UPROPERTY(blueprintReadWrite, editDefaultsOnly, category = "Action")
	FName EquipmentCategoryName;
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

	//Conditions applied to target
	UPROPERTY(blueprintReadWrite, editDefaultsOnly, category = "Action")
	TArray<FName> TargetConditions;

	////Conditions applied to Self
	//UPROPERTY(blueprintReadWrite, editDefaultsOnly, category = "Action")
	//TArray<FConditionStruct> SelfConditions;

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