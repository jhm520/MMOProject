// Fill out your copyright notice in the Description page of Project Settings.


#include "MMOActionComponent.h"
#include "MHelperLibrary.h"
#include "MMOCharacter.h"
#include "MActionInstance.h"

// Sets default values for this component's properties
UMMOActionComponent::UMMOActionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);

	// ...
}


// Called when the game starts
void UMMOActionComponent::BeginPlay()
{
	Super::BeginPlay();

	SetupActionMapNative();

	// ...
	
}

void UMMOActionComponent::SetupActionMapNative()
{
	AMMOCharacter* OwningChar = Cast<AMMOCharacter>(GetOwner());

	if (!OwningChar)
	{
		return;
	}

	TArray<FName>& ActionList = OwningChar->ActionList;

	for (FName& Action : ActionList)
	{
		FActionStruct NewAction;
		bool bFoundAction = UMHelperLibrary::GetActionWithName(this, Action, NewAction, nullptr);

		if (!bFoundAction)
		{
			return;
		}

		ActionMap.Add(Action, NewAction);
	}
}

void UMMOActionComponent::OnLeftCombatNative()
{
	AutoCastList.Empty();
}

void UMMOActionComponent::RequestDoAction_Implementation(const FName& InActionName, AActor* TargetActor, FVector TargetLocation)
{
	if(GetOwner()->GetLocalRole() < ROLE_Authority) {
		ServerRequestDoAction(InActionName, TargetActor, TargetLocation);
		return;
	}

	FActionStruct NewActionData;

	bool bFoundAction = UMHelperLibrary::GetActionWithName(this, InActionName, NewActionData, this);

	if (bFoundAction)
	{
		AuthDoAction(NewActionData, TargetActor, TargetLocation);
	}
	
}

void UMMOActionComponent::AuthDoAction(const FActionStruct& InActionStruct, AActor* TargetActor, FVector TargetLocation)
{
	//if we aren't the server, return
	if (GetOwner()->GetLocalRole() < ROLE_Authority)
	{
		return;
	}

	EActionFailureType ActionFailureType;
	bool bActionIsValid = IsActionValidNative(InActionStruct, TargetActor, TargetLocation, ActionFailureType);

	const EActionUseType& ActionUseType = InActionStruct.UseType;
	const EActionCastType& ActionCastType = InActionStruct.CastType;

	//if the action is not valid, it failed, try adding or removing it from the auto cast list
	if (!bActionIsValid)
	{
		switch (ActionUseType)
		{
			case EActionUseType::Toggle:
			case EActionUseType::AutoCast:
			{
				if (AutoCastList.Contains(InActionStruct.ActionName))
				{
					AutoCastList.Remove(InActionStruct.ActionName);
				}
				else
				{
					AutoCastList.AddUnique(InActionStruct.ActionName);
				}
				break;
			}
		}

		OnActionFailure(InActionStruct, ActionFailureType, true);
		return;
	}


	if (ActionUseType == EActionUseType::AutoCast)
	{
		AutoCastList.AddUnique(InActionStruct.ActionName);
	}

	switch (ActionCastType)
	{
		case EActionCastType::Instant:
		{
			//apply action
			ApplyActionNative(InActionStruct, TargetActor, TargetLocation);
			break;
		}
		case EActionCastType::Cast:
		{
			//apply action
			StartCastingNative(InActionStruct, TargetActor, TargetLocation);
			break;
		}
	}

	

}

void UMMOActionComponent::ApplyActionNative_Implementation(const FActionStruct& InActionStruct, AActor* TargetActor, FVector TargetLocation)
{

}

void UMMOActionComponent::StartCastingNative_Implementation(const FActionStruct& InActionStruct, AActor* TargetActor, FVector TargetLocation)
{
	
}

bool UMMOActionComponent::IsActionValidNative(const FActionStruct& InActionStruct, AActor* TargetActor, FVector TargetLocation, EActionFailureType& OutActionFailureType)
{
	//if we're already casting a spell, action is not valid
	if (IsCastingSpellNative())
	{
		OutActionFailureType = EActionFailureType::IsCasting;
		return false;
	}

	//if the target character is already dead, target is not valid, action is not valid
	AMMOCharacter* TargetMMOChar = Cast<AMMOCharacter>(TargetActor);

	if (TargetMMOChar && TargetMMOChar->IsDead())
	{
		OutActionFailureType = EActionFailureType::InvalidTarget;
		return false;
	}

	//if our own MMOChar is dead, action is not valid
	AMMOCharacter* OwningMMOChar = Cast<AMMOCharacter>(GetOwner());

	if (OwningMMOChar && OwningMMOChar->IsDead())
	{
		OutActionFailureType = EActionFailureType::Disabled;
		return false;
	}

	//if this action is already on cooldown, return
	AMActionInstance* CooldownActionInstance;

	if (IsActionOnCooldownNative(InActionStruct.ActionName, CooldownActionInstance))
	{
		OutActionFailureType = EActionFailureType::Cooldown;
		return false;
	}

	const EActionTargetType& ActionTargetType = InActionStruct.TargetType;

	AActor* LocalTargetActor = GetTargetActorNative();

	switch (ActionTargetType)
	{
		case EActionTargetType::NoTarget:
		case EActionTargetType::TargetSelf:
		case EActionTargetType::GroundTarget:
		{
			return true;
		}
		case EActionTargetType::TargetEnemy:
		{
			//if we targeted ourself, return
			if (TargetActor == GetOwner())
			{
				OutActionFailureType = EActionFailureType::InvalidTarget;
				return false;
			}

			if (IsActionInRangeNative(InActionStruct.ActionName, TargetActor))
			{
				return true;
			}
			else
			{
				OutActionFailureType = EActionFailureType::OutOfRange;
				return false;
			}
		}
		case EActionTargetType::Target:
		case EActionTargetType::TargetAlly:
		{
			//didn't have a target, valid if not enemy
			if (!TargetActor)
			{
				return true;
			}

			if (IsActionInRangeNative(InActionStruct.ActionName, TargetActor))
			{
				return true;
			}
			else
			{
				OutActionFailureType = EActionFailureType::OutOfRange;
				return false;
			}
		}
	}

	OutActionFailureType = EActionFailureType::DefaultFailure;
	return false;

}

bool UMMOActionComponent::IsActionInRangeNative(const FName& InActionName, AActor* TargetActor)
{
	FActionStruct ActionData;

	bool bFoundAction = UMHelperLibrary::GetActionWithName(this, InActionName, ActionData, this);

	if (!bFoundAction)
	{
		return false;
	}

	//if action has 0 range, its in range by default
	if (ActionData.CastMaxRange == 0.0f)
	{
		return true;
	}

	if (!TargetActor)
	{
		return false;
	}

	//if the target is within range of the owning character for this action, return true, else return false
	const FVector TargetLoc = TargetActor->GetActorLocation();

	const FVector CharLoc = GetOwner()->GetActorLocation();

	FVector ActionVect = TargetLoc - CharLoc;

	float ActionDist = ActionVect.Size();

	if (ActionDist < ActionData.CastMaxRange)
	{
		return true;
	}

	return false;
}

bool UMMOActionComponent::IsCastingSpellNative()
{
	return !CastingSpell.IsNone();
}

bool UMMOActionComponent::IsActionOnCooldownNative(const FName& InActionName, AMActionInstance*& OutActionInstance)
{
	AMActionInstance** FoundActionInstanceActor = ActionInstanceMap.Find(InActionName);

	if (FoundActionInstanceActor && *FoundActionInstanceActor)
	{
		OutActionInstance = *FoundActionInstanceActor;
		return true;
	}

	return false;
}

AActor* UMMOActionComponent::GetTargetActorNative()
{
	return Cast<AMMOCharacter>(GetOwner());
}

void UMMOActionComponent::ServerRequestDoAction_Implementation(const FName& InActionName, AActor* TargetActor, FVector TargetLocation)
{
	RequestDoAction(InActionName, TargetActor, TargetLocation);
}

bool UMMOActionComponent::ServerRequestDoAction_Validate(const FName& InActionName, AActor* TargetActor, FVector TargetLocation)
{
	return true;
}

void UMMOActionComponent::ClientOnActionFailure_Implementation(const FActionStruct& InActionStruct, EActionFailureType FailureReason)
{
	OnActionFailure(InActionStruct, FailureReason, false);
}

void UMMOActionComponent::OnActionFailure_Implementation(const FActionStruct& InActionStruct, EActionFailureType FailureReason, bool bNotifyServer)
{
	//Tell the client why this action failed
	if (GetOwner()->GetLocalRole() == ROLE_Authority && bNotifyServer)
	{
		ClientOnActionFailure(InActionStruct, FailureReason);
	}
}


void UMMOActionComponent::MulticastOnActionSuccess_Implementation(const FCharacterActionNotify& InActionNotify)
{
	OnActionSuccess(InActionNotify);
}

void UMMOActionComponent::OnActionSuccess_Implementation(const FCharacterActionNotify& InActionStruct)
{

}

// Called every frame
void UMMOActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//TickCooldownsNative(DeltaTime);

	//TickActionsNative(DeltaTime);




	// ...
}


void UMMOActionComponent::TickCooldownsNative(float DeltaTime)
{
	TArray<FName> ActionKeys;
	ActionInstanceMap.GetKeys(ActionKeys);

	const float CurrentTime = GetWorld()->GetTimeSeconds();

	TArray<FName> DoneCooldowns;

	for (FName& Action : ActionKeys)
	{
		AMActionInstance** FoundActionInstance = ActionInstanceMap.Find(Action);

		if (!FoundActionInstance)
		{
			continue;
		}

		AMActionInstance* ActionInstance = *FoundActionInstance;

		if (!ActionInstance)
		{
			continue;
		}

		const FCharacterActionNotify& ActionNotify = ActionInstance->ActionNotify;

		if (CurrentTime - ActionNotify.TimeOfCast > ActionNotify.Action.CooldownTime)
		{
			DoneCooldowns.Add(Action);
		}
	}

	for (FName& DoneCooldown : DoneCooldowns)
	{
		EndCooldownNative(DoneCooldown);
	}
}

void UMMOActionComponent::EndCooldownNative(const FName& InCooldownName)
{
	AMActionInstance** FoundActionInstance = ActionInstanceMap.Find(InCooldownName);

	if (!FoundActionInstance)
	{
		return;
	}

	AMActionInstance* ActionInstance = *FoundActionInstance;

	if (!ActionInstance)
	{
		return;
	}

	FCharacterActionNotify& ActionNotify = ActionInstance->ActionNotify;

	if (ActionInstance->bAppliedEffects)
	{
		ActionInstance->Destroy();
	}

	ActionInstanceMap.Remove(InCooldownName);

	OnCooldownEndedNative(ActionNotify);

}

void UMMOActionComponent::OnCooldownEndedNative_Implementation(const FCharacterActionNotify& InEndedAction)
{

}

void UMMOActionComponent::TickActionsNative(float DeltaTime)
{
	if (!HasAuthority())
	{
		return;
	}

	for (FName& AutoCastSpell : AutoCastList)
	{
		FActionStruct AutoCastAction;

		bool bFoundAction = UMHelperLibrary::GetActionWithName(this, AutoCastSpell, AutoCastAction, this);

		if (!bFoundAction)
		{
			continue;
		}

		AActor* TargetActor = GetTargetActorNative();

		if (!TargetActor)
		{
			continue;
		}

		EActionFailureType ActionFailure;

		bool bActionValid = IsActionValidNative(AutoCastAction, TargetActor, FVector::ZeroVector, ActionFailure);

		if (bActionValid)
		{
			AuthDoAction(AutoCastAction, TargetActor, FVector::ZeroVector);
		}
	}
}

bool UMMOActionComponent::HasAuthority()
{
	return GetOwner() && GetOwner()->HasAuthority();
}

void UMMOActionComponent::TickCastingNative(float DeltaTime)
{

}

void UMMOActionComponent::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UMMOActionComponent, CastingSpell);
	DOREPLIFETIME(UMMOActionComponent, AutoCastList);


}