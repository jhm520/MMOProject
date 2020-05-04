// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MMOUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class MMOPROJECT_API UMMOUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	//The actor that this widget is attached to (not the local player)
	UPROPERTY(Transient, BlueprintReadWrite, Category = "Widget")
	AActor* ParentActor = nullptr;
};
