// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MHUD.generated.h"

UENUM(BlueprintType)
enum class EHudUpdateType : uint8
{
	All,
	Health,
	Mana,
	Name,
	TargetAll,
	TargetHealth,
	TargetName,
	TargetMana,
	TargetTarget
};

/**
 * 
 */
UCLASS()
class MMOPROJECT_API AMHUD : public AHUD
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Hud")
	void UpdateHUD(EHudUpdateType UpdateType = EHudUpdateType::All);
	
};
