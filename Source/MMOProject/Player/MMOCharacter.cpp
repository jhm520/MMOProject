// Fill out your copyright notice in the Description page of Project Settings.

#include "MMOCharacter.h"
#include "MMOProject.h"
#include "GameFramework\CharacterMovementComponent.h"
#include "Camera\CameraComponent.h"
#include "GameFramework\PlayerController.h"
#include "Components\StaticMeshComponent.h"
#include "Components\SkeletalMeshComponent.h"

// Sets default values
AMMOCharacter::AMMOCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	CameraRoot = ObjectInitializer.CreateDefaultSubobject<USceneComponent>(this, TEXT("CameraRoot"));
	CameraRoot->SetupAttachment(GetRootComponent());

	PlayerCameraComponent = ObjectInitializer.CreateDefaultSubobject<UCameraComponent>(this, TEXT("PlayerCamera"));
	PlayerCameraComponent->SetupAttachment(CameraRoot);

	TargetingCircleMesh = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("TargetingCircleMesh"));

	TargetingCircleMesh->SetupAttachment(GetRootComponent());



}

// Called when the game starts or when spawned
void AMMOCharacter::BeginPlay()
{
	Super::BeginPlay();

	
	//GetMesh()->OnClicked.AddDynamic(this, &AMMOCharacter::OnMeshClicked);
	
}

// Called every frame
void AMMOCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bWantsToControlCamera)
	{
		CameraRoot->SetWorldLocation(GetRootComponent()->GetComponentLocation());
	}
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
	FVector MoveVector = GetActorForwardVector() *	InAxis;
	GetCharacterMovement()->AddInputVector(GetActorForwardVector()*	InAxis, false);
}

void AMMOCharacter::TurnRight(float InAxis)
{
	if (bWantsToControlPlayer)
	{
		GetCharacterMovement()->AddInputVector(GetActorRightVector()*InAxis, false);
	}
	else
	{
		AddControllerYawInput(InAxis);
	}
}

void AMMOCharacter::LookRight(float InAxis)
{
	if (bWantsToControlPlayer)
	{
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
		const FRotator RelativeRotation = CameraRoot->RelativeRotation;


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
			IMMOClickableInterface::Execute_OnStoppedTargeting(TargetActor, Cast<APlayerController>(GetController()), this);
			TargetActor = nullptr;
		}

		IMMOClickableInterface::Execute_OnTargeted(MouseHit.GetActor(), Cast<APlayerController>(GetController()), this);
		TargetActor = MouseHit.GetActor();
	}
	else
	{
		if (TargetActor)
		{
			IMMOClickableInterface::Execute_OnStoppedTargeting(TargetActor, Cast<APlayerController>(GetController()), this);
			TargetActor = nullptr;
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

	GetController()->SetControlRotation(FRotator(0.0f, CameraRoot->GetComponentRotation().Yaw, 0.0f));
	CameraRoot->SetRelativeRotation(FRotator(CameraRoot->RelativeRotation.Pitch, 0.0f, CameraRoot->RelativeRotation.Roll));

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
	PlayerCameraComponent->SetRelativeLocation(FVector(FMath::Clamp(PlayerCameraComponent->RelativeLocation.X + 10, MaxCameraZoomDistance, MinCameraZoomDistance), 0.0f, 0.0f));
}

void AMMOCharacter::OnCameraZoomOut()
{
	PlayerCameraComponent->SetRelativeLocation(FVector(FMath::Clamp(PlayerCameraComponent->RelativeLocation.X - 10, MaxCameraZoomDistance, MinCameraZoomDistance), 0.0f, 0.0f));
}

void AMMOCharacter::OnJumpPressed()
{
	Jump();
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
}

void AMMOCharacter::OnStoppedTargeting_Implementation(APlayerController* EventInstigator, AActor* TargetedBy)
{
	TargetingCircleMesh->SetHiddenInGame(true);
}

