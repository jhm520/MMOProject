// Fill out your copyright notice in the Description page of Project Settings.


#include "MActionDataAsset.h"
#include "MGameInstance.h"

bool UMActionDataAsset::GetConditionStruct(UObject* WorldContextObject, const FName& InConditionName, FConditionStruct& OutConditionStruct)
{
	OutConditionStruct = FConditionStruct();

	if (!WorldContextObject)
	{
		return false;
	}

	UMGameInstance* GameInst = WorldContextObject->GetWorld()->GetGameInstance<UMGameInstance>();

	if (!GameInst)
	{
		return false;
	}

	UMActionDataAsset* CondDataAsset = Cast<UMActionDataAsset>(GameInst->ActionDataAsset);

	if (!CondDataAsset)
	{
		return false;
	}

	for (FConditionStruct& Condition : CondDataAsset->Conditions)
	{
		if (InConditionName == Condition.ConditionName)
		{
			OutConditionStruct = Condition;
			return true;
		}
	}

	return false;
}
