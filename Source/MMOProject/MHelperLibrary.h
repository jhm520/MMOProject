// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MMOTypes.h"
#include "MHelperLibrary.generated.h"

/**
 * 
 */
UCLASS()
class MMOPROJECT_API UMHelperLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()


	//Get the action data from this action name, optionally relevant to the supplied action component
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Action", meta = (WorldContext = "WorldContextObject"))
	static bool GetActionWithName(UObject* WorldContextObject, const FName& InActionName, FActionStruct& OutActionData, UMMOActionComponent* ActionComponent = nullptr);
	
};
