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

bool UMActionDataAsset::GetEquipmentStruct(UObject* WorldContextObject, const FName& InEquipmentName, FEquipmentStruct& OutEquipmentStruct)
{
	if (!WorldContextObject)
	{
		return false;
	}

	UMGameInstance* GameInst = WorldContextObject->GetWorld()->GetGameInstance<UMGameInstance>();

	if (!GameInst)
	{
		return false;
	}

	UDataTable* EquipDataTable = GameInst->EquipmentDataTable;

	if (!EquipDataTable)
	{
		return false;
	}

	TArray<FName> DataTableRowNames = EquipDataTable->GetRowNames();

	FString ContextString;

	for (FName RowName : DataTableRowNames)
	{
		FEquipmentStruct* StructPtr = EquipDataTable->FindRow<FEquipmentStruct>(RowName, ContextString);

		if (!StructPtr)
		{
			continue;
		}

		//Load up all the weapon properties structs from this data table into this weapon
		if (StructPtr->EquipmentName == InEquipmentName)
		{
			OutEquipmentStruct = *StructPtr;
			return true;
		}
	}

	return false;


}
