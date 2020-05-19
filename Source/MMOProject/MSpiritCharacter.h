// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/MMOCharacter.h"
#include "MSpiritCharacter.generated.h"

/**
 * 
 */
UCLASS()
class MMOPROJECT_API AMSpiritCharacter : public AMMOCharacter
{
	GENERATED_BODY()

public:
    virtual bool IsDead() override;

};
