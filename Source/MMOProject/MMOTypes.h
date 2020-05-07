// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine.h"
#include "Engine/EngineTypes.h"
#include "MMOTypes.generated.h"

USTRUCT(blueprintType)
struct FActionStruct : public FTableRowBase
{
	GENERATED_BODY()
public:

	UPROPERTY(blueprintReadWrite, editDefaultsOnly, category="Action")
		FName ActionName;

	UPROPERTY(blueprintReadWrite, editDefaultsOnly, category = "Action")
		float damage;

};