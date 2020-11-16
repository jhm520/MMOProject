// Fill out your copyright notice in the Description page of Project Settings.

#include "MMOCharacter.h"
#include "MMOProject.h"
#include "GameFramework\CharacterMovementComponent.h"
#include "Engine.h"
#include "Camera\CameraComponent.h"
#include "GameFramework\PlayerController.h"
#include "Components\StaticMeshComponent.h"
#include "Components\SkeletalMeshComponent.h"
#include "MMOWidgetComponent.h"
#include "MMOUserWidget.h"
#include "Kismet/KismetMathLibrary.h"
#include "MMOTypes.h"

// Sets default values
AMMOCharacter::AMMOCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	bReplicates = true;

	CameraRoot = ObjectInitializer.CreateDefaultSubobject<USceneComponent>(this, TEXT("CameraRoot"));
	CameraRoot->SetupAttachment(GetRootComponent());

	PlayerCameraComponent = ObjectInitializer.CreateDefaultSubobject<UCameraComponent>(this, TEXT("PlayerCamera"));
	PlayerCameraComponent->SetupAttachment(CameraRoot);

	TargetingCircleMesh = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("TargetingCircleMesh"));

	TargetingCircleMesh->SetupAttachment(GetRootComponent());
	TargetingCircleMesh->SetHiddenInGame(true);
	TargetingCircleMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);


	if (GetCapsuleComponent())
	{
		NameplateWidgetComponent = ObjectInitializer.CreateDefaultSubobject<UMMOWidgetComponent>(this, TEXT("NameplateWidget"));
		NameplateWidgetComponent->SetWidgetClass(UMMOUserWidget::StaticClass());
		NameplateWidgetComponent->SetupAttachment(GetCapsuleComponent());
		NameplateWidgetComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 120.0f));
		NameplateWidgetComponent->SetDrawSize(FVector2D(500.0f, 100.0f));
		NameplateWidgetComponent->SetHiddenInGame(true);
	}
}

// Called when the game starts or when spawned
void AMMOCharacter::BeginPlay()
{
	Super::BeginPlay();

	SpawnNameplate();
	//GetMesh()->OnClicked.AddDynamic(this, &AMMOCharacter::OnMeshClicked);
	
}

void AMMOCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{

	//Clean Up NameplateWidget
	if (NameplateWidgetComponent)
	{
		if (NameplateWidgetComponent->GetUserWidgetObject())
		{
			NameplateWidgetComponent->GetUserWidgetObject()->Destruct();
		}

		NameplateWidgetComponent->DestroyComponent();

		NameplateWidgetComponent = nullptr;
		
	}

	Super::EndPlay(EndPlayReason);
}

// Called every frame
void AMMOCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bWantsToControlCamera)
	{
		CameraRoot->SetWorldLocation(GetRootComponent()->GetComponentLocation());
	}

	//if (!bUseControllerRotationYaw && !Getwor)
	//{
	//	SetActorRotation(GetControlRotation());
	//}

	TickNameplate();
}

float AMMOCharacter::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	if (!IsConditionDamage(DamageEvent))
	{
		Health = FMath::Min(MaxHealth, FMath::Max(0.0f, Health - Damage));
		OnRep_Health();
	}

	if (!IsDead())
	{
		AddThreat(DamageCauser, 0.0f, true);
	}

	return Damage;
}

bool AMMOCharacter::IsConditionDamage(struct FDamageEvent const& DamageEvent)
{
	return DamageEvent.DamageTypeClass->IsChildOf(UMConditionDamage::StaticClass());
}

void AMMOCharacter::OnDeath_Implementation()
{
	if (GetWorld()->IsServer())
	{
		RemoveAllThreats();
	}
}

void AMMOCharacter::AddThreat(AActor* InThreat, float InThreatLevel, bool bAffectOther)
{
	bool bRep = false;

	if (!Threats.Contains(InThreat))
	{
		bRep = true;
	}

	Threats.AddUnique(InThreat);

	float* ThreatPtr = ThreatMap.Find(InThreat);

	if (ThreatPtr)
	{
		*ThreatPtr += InThreatLevel;
	}
	else
	{
		ThreatMap.Add(InThreat, InThreatLevel);
	}

	OnRep_Threats();
	

	

	AMMOCharacter* NewThreatChar = Cast<AMMOCharacter>(InThreat);

	if (NewThreatChar && bAffectOther)
	{
		NewThreatChar->AddThreat(this, InThreatLevel);
	}
}

void AMMOCharacter::RemoveThreat(AActor* InThreat, bool bAffectOther)
{
	AMMOCharacter* NewThreatChar = Cast<AMMOCharacter>(InThreat);

	if (NewThreatChar && bAffectOther)
	{
		NewThreatChar->RemoveThreat(this);
	}

	Threats.Remove(InThreat);
	ThreatMap.Remove(InThreat);
	OnRep_Threats();
}

AActor* AMMOCharacter::GetHighestThreatTarget()
{
	float LocalHighestThreat = -1.0f;

	AActor* LocalHighestThreatTarget = nullptr;

	for (AActor* Threat : Threats)
	{
		float* ThreatPtr = ThreatMap.Find(Threat);

		if (!ThreatPtr)
		{
			continue;
		}

		if (*ThreatPtr > LocalHighestThreat)
		{
			LocalHighestThreat = *ThreatPtr;
			LocalHighestThreatTarget = Threat;
		}
	}

	return LocalHighestThreatTarget;
}

void AMMOCharacter::RemoveAllThreats()
{
	for (AActor* Threat : Threats)
	{
		AMMOCharacter* NewThreatChar = Cast<AMMOCharacter>(Threat);

		if (NewThreatChar)
		{
			NewThreatChar->RemoveThreat(this);
		}
	}

	ThreatMap.Empty();
	Threats.Empty();
	OnRep_Threats();
}

void AMMOCharacter::TickNameplate()
{
	if (!NameplateWidgetComponent)
	{
		return;
	}

	APlayerController* LocalPlayerController = UGameplayStatics::GetPlayerController(this, 0);

	if (!LocalPlayerController)
	{
		return;
	}

	const FVector NameplateLoc = NameplateWidgetComponent->GetComponentLocation();

	const FVector CameraLoc = LocalPlayerController->PlayerCameraManager->GetTransformComponent()->GetComponentLocation();

	FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(NameplateLoc, CameraLoc);

	NameplateWidgetComponent->SetWorldRotation(LookAtRotation);
	
}

// Called to bind functionality to input
void AMMOCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	InputComponent->BindAxis("MoveForward", this, &AMMOCharacter::MoveForward);

	InputComponent->BindAxis("TurnRight", this, &AMMOCharacter::TurnRight);

	InputComponent->BindAxis("LookRight", this, &AMMOCharacter::LookRight);

	InputComponent->BindAxis("LookUp", this, &AMMOCharacter::LookUp);

	InputComponent->BindAction("ControlCamera", IE_Pressed, this, &AMMOCharacter::OnControlCameraPressed);

	InputComponent->BindAction("ControlCamera", IE_Released, this, &AMMOCharacter::OnControlCameraReleased);

	InputComponent->BindAction("ControlPlayer", IE_Pressed, this, &AMMOCharacter::OnControlPlayerPressed);

	InputComponent->BindAction("ControlPlayer", IE_Released, this, &AMMOCharacter::OnControlPlayerReleased);

	InputComponent->BindAction("ZoomInCamera", IE_Pressed, this, &AMMOCharacter::OnCameraZoomIn);

	InputComponent->BindAction("ZoomOutCamera", IE_Pressed, this, &AMMOCharacter::OnCameraZoomOut);

	InputComponent->BindAction("Jump", IE_Pressed, this, &AMMOCharacter::OnJumpPressed);


}

void AMMOCharacter::MoveForward(float InAxis)
{
	if (IsDead())
	{
		return;
	}

	FVector MoveVector = GetActorForwardVector() *	InAxis;
	GetCharacterMovement()->AddInputVector(GetActorForwardVector()*	InAxis, false);
}

void AMMOCharacter::TurnRight(float InAxis)
{
	if (bWantsToControlPlayer)
	{
		if (IsDead())
		{
			return;
		}

		GetCharacterMovement()->AddInputVector(GetActorRightVector()*InAxis, false);
	}
	else
	{
		if (IsDead())
		{
			return;
		}

		AddControllerYawInput(InAxis);
	}
}

void AMMOCharacter::LookRight(float InAxis)
{
	if (bWantsToControlPlayer)
	{
		if (IsDead())
		{
			CameraRoot->AddLocalRotation(FRotator(0.0f, InAxis, 0.0f));
			return;
		}

		AddControllerYawInput(InAxis);
	}
	else if (bWantsToControlCamera)
	{
		CameraRoot->AddLocalRotation(FRotator(0.0f, InAxis, 0.0f));
	}
}

void AMMOCharacter::LookUp(float InAxis)
{
	if (bWantsToControlPlayer || bWantsToControlCamera)
	{
		const FRotator RelativeRotation = CameraRoot->GetRelativeRotation();


		const float NewRelativePitch = FMath::Clamp(RelativeRotation.Pitch + InAxis, -89.0f, 89.0f);

		CameraRoot->SetRelativeRotation(FRotator(NewRelativePitch, RelativeRotation.Yaw, 0.0f));
	}
}

void AMMOCharacter::OnMeshClicked(UPrimitiveComponent* TouchedComponent, FKey ButtonPressed)
{
	IMMOClickableInterface::Execute_OnTargeted(this, Cast<APlayerController>(GetController()), this);
}

void AMMOCharacter::OnControlCameraPressed()
{

	bWantsToControlCamera = true;

	APlayerController* PlayerController = Cast<APlayerController>(GetController());

	if (!PlayerController)
	{
		return;
	}

	FHitResult MouseHit;


	//Handle Targeting
	bool bFoundMouseHit = PlayerController->GetHitResultUnderCursor(ECC_Visibility, false, MouseHit);

	if (bFoundMouseHit && MouseHit.GetActor() && MouseHit.GetActor()->GetClass()->ImplementsInterface(UMMOClickableInterface::StaticClass()))
	{
		if (TargetActor && MouseHit.GetActor() != TargetActor)
		{
			SetTargetActor(nullptr);
		}

		SetTargetActor(MouseHit.GetActor());
	}
	else
	{
		if (TargetActor)
		{
			SetTargetActor(nullptr);
		}
	}



	CameraControlRotator = CameraRoot->GetComponentRotation();

	PlayerController->bShowMouseCursor = false;
	
	CameraRoot->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld, true));
}

void AMMOCharacter::OnControlCameraReleased()
{
	bWantsToControlCamera = false;

	APlayerController* PlayerController = Cast<APlayerController>(GetController());

	if (!PlayerController)
	{
		return;
	}

	if (!bWantsToControlPlayer)
	{
		PlayerController->bShowMouseCursor = true;
	}

	CameraRoot->AttachToComponent(GetRootComponent(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, true));
}

void AMMOCharacter::OnControlPlayerPressed()
{
	bWantsToControlPlayer = true;

	if (!IsDead())
	{
		GetController()->SetControlRotation(FRotator(0.0f, CameraRoot->GetComponentRotation().Yaw, 0.0f));
	}


	CameraRoot->SetRelativeRotation(FRotator(CameraRoot->GetRelativeRotation().Pitch, 0.0f, CameraRoot->GetRelativeRotation().Roll));

	APlayerController* PlayerController = Cast<APlayerController>(GetController());

	if (!PlayerController)
	{
		return;
	}

	
	PlayerController->bShowMouseCursor = false;
}

void AMMOCharacter::OnControlPlayerReleased()
{
	bWantsToControlPlayer = false;

	APlayerController* PlayerController = Cast<APlayerController>(GetController());

	if (!PlayerController)
	{
		return;
	}

	if (!bWantsToControlCamera)
	{
		PlayerController->bShowMouseCursor = true;
	}
}

void AMMOCharacter::OnCameraZoomIn()
{
	PlayerCameraComponent->SetRelativeLocation(FVector(FMath::Clamp(PlayerCameraComponent->GetRelativeLocation().X + 10, MaxCameraZoomDistance, MinCameraZoomDistance), 0.0f, 0.0f));
}

void AMMOCharacter::OnCameraZoomOut()
{
	PlayerCameraComponent->SetRelativeLocation(FVector(FMath::Clamp(PlayerCameraComponent->GetRelativeLocation().X - 10, MaxCameraZoomDistance, MinCameraZoomDistance), 0.0f, 0.0f));
}

void AMMOCharacter::OnJumpPressed()
{
	Jump();
}

void AMMOCharacter::OnRep_FactionName()
{

}

EFactionRelationStatus AMMOCharacter::GetFactionRelation(AActor* OtherActor)
{
	AMMOCharacter* MChar = Cast<AMMOCharacter>(OtherActor);

	if (!MChar)
	{
		return EFactionRelationStatus::Neutral;
	}

	EFactionRelationStatus* RelationPtr = FactionRelationMap.Find(MChar->FactionName);

	if (!RelationPtr)
	{
		return EFactionRelationStatus::Neutral;
	}

	return *RelationPtr;
}

void AMMOCharacter::OnRep_FactionRelationMap()
{

}

float AMMOCharacter::GetHealthPercentage()
{
	return Health / MaxHealth;
}

FString AMMOCharacter::GetFriendlyName_Implementation()
{
	return CharacterName;
}

void AMMOCharacter::OnLeftClicked_Implementation(APlayerController* EventInstigator)
{

}

void AMMOCharacter::OnRightClicked_Implementation(APlayerController* EventInstigator)
{

}

void AMMOCharacter::OnTargeted_Implementation(APlayerController* EventInstigator, AActor* TargetedBy)
{
	TargetingCircleMesh->SetHiddenInGame(false);
	NameplateWidgetComponent->SetHiddenInGame(false);
}

void AMMOCharacter::OnStoppedTargeting_Implementation(APlayerController* EventInstigator, AActor* TargetedBy)
{
	TargetingCircleMesh->SetHiddenInGame(true);
	NameplateWidgetComponent->SetHiddenInGame(true);
}


void AMMOCharacter::SpawnNameplate()
{

	if (!NameplateWidgetClass || !NameplateWidgetClass->IsChildOf<UUserWidget>())
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Yellow, FString("Could not spawn HealthBarWidget, class invalid"));
		return;
	}

	if (!NameplateWidgetComponent)
	{
		return;
	}

	NameplateWidgetComponent->SetWidgetClass(NameplateWidgetClass);

	UMMOUserWidget* Widget = Cast<UMMOUserWidget>(NameplateWidgetComponent->GetUserWidgetObject());

	if (Widget)
	{
		Widget->ParentActor = this;
	}
}

bool AMMOCharacter::IsDead()
{
	return Health <= 0.0f;
}

void AMMOCharacter::OnRep_bIsInCombat()
{

}

void AMMOCharacter::OnRep_Health()
{
	OnHealthChanged();
}

void AMMOCharacter::OnHealthChanged_Implementation()
{
	if (IsDead())
	{
		OnDeath();
	}
	UpdatePlayerHUD(EHudUpdateType::Health);
	UpdateTargetHUD(EHudUpdateType::TargetHealth);
}

void AMMOCharacter::UpdatePlayerHUD(EHudUpdateType UpdateType /*= EHudUpdateType::All*/)
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());

	if (PlayerController && PlayerController->IsLocalController())
	{
		AMHUD* MHud = Cast<AMHUD>(PlayerController->GetHUD());

		if (MHud)
		{
			MHud->UpdateHUD(UpdateType);
		}
	}
}

void AMMOCharacter::UpdateTargetHUD(EHudUpdateType UpdateType /*= EHudUpdateType::All*/)
{
	//get the local player controller
	APlayerController* LocalPlayerController = UGameplayStatics::GetPlayerController(this, 0);

	if (!LocalPlayerController)
	{
		return;
	}

	AMMOCharacter* LocalChar = Cast<AMMOCharacter>(LocalPlayerController->GetPawn());

	if (!LocalChar)
	{
		return;
	}

	//if this is our target actor, update it
	if (LocalChar->TargetActor && LocalChar->TargetActor == this)
	{
		LocalChar->UpdatePlayerHUD(UpdateType);
	}
}

float AMMOCharacter::GetManaPercentage()
{
	return Mana/MaxMana;
}

void AMMOCharacter::OnRep_Mana()
{
	OnManaChanged();
}

void AMMOCharacter::OnManaChanged_Implementation()
{
	UpdatePlayerHUD(EHudUpdateType::Mana);
	UpdateTargetHUD(EHudUpdateType::TargetMana);
}

void AMMOCharacter::OnRep_TargetActor()
{
	OnTargetActorChanged();
}

void AMMOCharacter::OnTargetActorChanged_Implementation()
{
	if (IsLocallyControlled() && TargetActor)
	{
		IMMOClickableInterface::Execute_OnTargeted(TargetActor, Cast<APlayerController>(GetController()), this);
	}

	UpdatePlayerHUD(EHudUpdateType::TargetAll);
	UpdateTargetHUD(EHudUpdateType::TargetTarget);

}

void AMMOCharacter::SetTargetActor(AActor* InTargetActor)
{
	if (GetLocalRole() < ROLE_Authority)
	{
		Server_SetTargetActor(InTargetActor);
	}

	if (IsLocallyControlled() && TargetActor)
	{
		IMMOClickableInterface::Execute_OnStoppedTargeting(TargetActor, Cast<APlayerController>(GetController()), this);
	}

	TargetActor = InTargetActor;
	OnRep_TargetActor();
}

void AMMOCharacter::Server_SetTargetActor_Implementation(AActor* InTargetActor)
{
	SetTargetActor(InTargetActor);
}

bool AMMOCharacter::Server_SetTargetActor_Validate(AActor* InTargetActor)
{
	return true;
}

void AMMOCharacter::OnRep_Threats()
{
	OnThreatsChanged();
}

void AMMOCharacter::OnThreatsChanged_Implementation()
{

}

bool AMMOCharacter::IsInCombat()
{
	return !IsDead() && Threats.Num() > 0;
}

void AMMOCharacter::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMMOCharacter, TargetActor);
	DOREPLIFETIME(AMMOCharacter, Health);
	DOREPLIFETIME(AMMOCharacter, Mana);
	DOREPLIFETIME(AMMOCharacter, Threats);
	DOREPLIFETIME(AMMOCharacter, FactionName);
	DOREPLIFETIME(AMMOCharacter, FactionRelationMap);

}

