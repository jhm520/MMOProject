// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "MMOClickableInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UMMOClickableInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MMOPROJECT_API IMMOClickableInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	void OnLeftClicked(APlayerController* EventInstigator);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	void OnRightClicked(APlayerController* EventInstigator);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	void OnTargeted(APlayerController* EventInstigator, AActor* TargetedBy);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	void OnStoppedTargeting(APlayerController* EventInstigator, AActor* TargetedBy);

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
};
