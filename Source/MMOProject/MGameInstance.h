// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Engine/DataTable.h"
#include "MGameInstance.generated.h"

/**
 * 
 */
UCLASS(blueprintable, blueprintType)
class MMOPROJECT_API UMGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Action")
	UDataTable* ActionDataTable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Action")
	UDataAsset* ActionDataAsset;
	
};
