// Fill out your copyright notice in the Description page of Project Settings.


#include "MHelperLibrary.h"
#include "MGameInstance.h"
#include "Player/MMOActionComponent.h"

bool UMHelperLibrary::GetActionWithName(UObject* WorldContextObject, const FName& InActionName, FActionStruct& OutActionData, UMMOActionComponent* ActionComponent /*= nullptr*/)
{
	if (!WorldContextObject)
	{
		return false;
	}

	if (ActionComponent)
	{
		FActionStruct* ActionPtr = ActionComponent->ActionMap.Find(InActionName);

		if (ActionPtr)
		{
			OutActionData = *ActionPtr;
			return true;
		}
	}

	UMGameInstance* GameInst = WorldContextObject->GetWorld()->GetGameInstance<UMGameInstance>();

	if (!GameInst)
	{
		return false;
	}

	UDataTable* ActionTable = GameInst->ActionDataTable;

	if (!ActionTable)
	{
		return false;
	}

	TArray<FActionStruct*> Actions;
	ActionTable->GetAllRows<FActionStruct>(FString(), Actions);

	for (FActionStruct* ActionPtr : Actions)
	{
		if (!ActionPtr)
		{
			continue;
		}

		if (ActionPtr->ActionName == InActionName)
		{
			OutActionData = *ActionPtr;
			return true;
		}
	}

	return false;

	//WorldContextObject->GetWorld()->GetGameInstance<()
}
