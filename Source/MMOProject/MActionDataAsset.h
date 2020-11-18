// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "MMOTypes.h"
#include "MActionDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class MMOPROJECT_API UMActionDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Conditions")
	TArray<FConditionStruct> Conditions;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Action", meta = (WorldContext = "WorldContextObject"))
	static bool GetConditionStruct(UObject* WorldContextObject, const FName& InConditionName, FConditionStruct& OutConditionStruct);

	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Action", meta = (WorldContext = "WorldContextObject"))
	static bool GetEquipmentStruct(UObject* WorldContextObject, const FName& InEquipmentName, FEquipmentStruct& OutEquipmentStruct);
};
