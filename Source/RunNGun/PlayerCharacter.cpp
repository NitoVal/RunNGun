// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

#include "Coin.h"
#include "Laser.h"
#include "Door.h"
#include "HealthComponent.h"
#include "NiagaraComponent.h"
#include "WeaponComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/ArrowComponent.h"
#include "Components/TimelineComponent.h"
#include "Engine/LocalPlayer.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{

 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	Firepoint = CreateDefaultSubobject<UArrowComponent>("Firepoint");
	Firepoint->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	
	AimingCameraTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("AimingCameraTimeline"));
	DefaultCameraLocation = FVector{0.f,0.f,100.f};
	AimingCameraLocation = FVector { 300.f, 50.f, 55.f};
	CameraBoom->SocketOffset = DefaultCameraLocation;

	HealthComponent = CreateDefaultSubobject<UHealthComponent>("HealthComponent");
	WeaponComponent = CreateDefaultSubobject<UWeaponComponent>("WeaponComponent");
	
	DashNiagaraSystem = CreateDefaultSubobject<UNiagaraComponent>(TEXT("DashNiagaraSystem"));
	DashNiagaraSystem->SetupAttachment(RootComponent);
	DashNiagaraSystem->bAutoActivate = false;
	
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	Coins = 0;

	bIsTargeting = false;
	
	FOnTimelineFloat AimLerpAlphaValue;
	FOnTimelineEvent TimelineFinishedEvent;
	AimLerpAlphaValue.BindUFunction(this, FName("UpdateCameraTimeline"));
	
	HealthComponent->OnDeath.AddDynamic(this, &APlayerCharacter::HandleDeath);
	
	if (AimingCameraTimeline && AimingCameraCurve)
	{
		AimingCameraTimeline->AddInterpFloat(AimingCameraCurve, AimLerpAlphaValue);
		AimingCameraTimeline->SetTimelineFinishedFunc(TimelineFinishedEvent);
	}
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	InteractionCheck();
	const float GravityDirection = GetGravityDirection().Z;
	if (!bIsAiming && bIsSprinting && GetVelocity().Size() == 0)
	{
		bIsSprinting = false;
		GetCharacterMovement()->MaxWalkSpeed /= 2;
	}
	DefaultCameraLocation = GravityDirection > 0 ? FVector(0,0,-100) : FVector(0,0,100);
	AimingCameraLocation = GravityDirection > 0 ? FVector(300,-50,-55) : FVector(300,50,55);
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	if (const APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}
	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);

		// Sprinting
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &APlayerCharacter::ToggleSprint);

		// Interact
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Interact);
		
		//Pause
		//EnhancedInputComponent->BindAction(PauseAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Pause);

		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, &APlayerCharacter::Fire);
		
		EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Started, this, &APlayerCharacter::Aim);
		EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Completed, this, &APlayerCharacter::StopAiming);

		//EnhancedInputComponent->BindAction(SwitchWeaponAction, ETriggerEvent::Started, this, &APlayerCharacter::SwitchWeapon);
		
		EnhancedInputComponent->BindAction(DashAction, ETriggerEvent::Completed, this, &APlayerCharacter::Dash);
	}
}

void APlayerCharacter::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	if (OtherActor->IsA(ACoin::StaticClass()))
	{
		ACoin* Coin = Cast<ACoin>(OtherActor);
		if (Coin)
		{
			Coins += Coin->CoinValue;
			Coin->Destroy();
		}
	}
	
	//TODO: Replace this to be when hitting any traps
	if (OtherActor->IsA(ALaser::StaticClass()))
	{
		HealthComponent->TakeDamage(HealthComponent->MaxHp);
	}
}


void APlayerCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();
	const float GravityDirection = GetGravityDirection().Z;
	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, GravityDirection > 0 ? -MovementVector.X : MovementVector.X);
	}
}

void APlayerCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();
	const float GravityDirection = GetGravityDirection().Z;
	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput( GravityDirection > 0 ? -LookAxisVector.X : LookAxisVector.X);
		AddControllerPitchInput( GravityDirection > 0 ? -LookAxisVector.Y : LookAxisVector.Y);
	}
}

void APlayerCharacter::ToggleSprint()
{
	if (!bIsAiming)
	{
		if (!bIsSprinting)
		{
			bIsSprinting = true;
			GetCharacterMovement()->MaxWalkSpeed = 1000.f;
		}
		else
		{
			bIsSprinting = false;
			GetCharacterMovement()->MaxWalkSpeed = 500.f;
		}
	}
}

void APlayerCharacter::Interact()
{
	if (CurrentInteractable)
		CurrentInteractable->Interact(this);
}

void APlayerCharacter::InteractionCheck()
{
	const FVector ViewLocation = GetPawnViewLocation();
	const FVector ViewDirection = GetViewRotation().Vector();
	const FVector TraceEnd = ViewLocation + ViewDirection * 250.f;
	
	if (FVector::DotProduct(GetActorForwardVector(), ViewDirection) <= 0)
	{
		CurrentInteractable = nullptr;
		bIsTargeting = false;
		return;
	}
	
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
    
	FHitResult TraceHit;
	if (!GetWorld()->LineTraceSingleByChannel(TraceHit, ViewLocation, TraceEnd, ECC_Visibility, QueryParams))
	{
		CurrentInteractable = nullptr;
		bIsTargeting = false;
		return;
	}
	
	AActor* HitActor = TraceHit.GetActor();
	if (HitActor && HitActor->GetClass()->ImplementsInterface(UInteractionInterface::StaticClass()))
	{
		if (HitActor != CurrentInteractable.GetObject())
		{
			CurrentInteractable = HitActor;
			bIsTargeting = true;
			return;
		}
		return;
	}
	
	CurrentInteractable = nullptr;
	bIsTargeting = false;
}

void APlayerCharacter::Dash()
{
	if (!bIsDashing && !GetVelocity().IsZero())
	{
		const float DashDistance = 5000.0f; 
		const float DashTime = 0.5f;

		const FVector CurrentVelocity = GetVelocity();
		const FVector DashDirection = FVector(CurrentVelocity.X, CurrentVelocity.Y, 0.0f).GetSafeNormal();
		const FVector DashVelocity = DashDirection * DashDistance / DashTime;
		
		DashNiagaraSystem->Activate();
		bIsDashing = true;
		LaunchCharacter(DashVelocity, false, false);
		GetWorldTimerManager().SetTimer(DashVFXTimer, this, &APlayerCharacter::DeactivateDashVFX,.15, false);
		GetWorldTimerManager().SetTimer(DelayDash, this, &APlayerCharacter::SetDashState,DashCooldown, false); //Start cooldown timer
	}
}

void APlayerCharacter::SetDashState()
{
	bIsDashing = false;
}

void APlayerCharacter::DeactivateDashVFX() const
{
	DashNiagaraSystem->Deactivate();
}

void APlayerCharacter::Fire()
{
	bUseControllerRotationYaw = true;
	// WeaponComponent->FireHitscan();
	WeaponComponent->FireProjectile(Firepoint);
	if (!bIsAiming)
	{
		GetWorldTimerManager().SetTimer(StopFiringTimer, this, &APlayerCharacter::StopFiring,.25, false);
	}
}

void APlayerCharacter::StopFiring()
{
	bUseControllerRotationYaw = false;
}

void APlayerCharacter::Aim()
{
	bIsAiming = true;
	bUseControllerRotationYaw = true;
	bIsSprinting = false;
	GetCharacterMovement()->MaxWalkSpeed = 400.f;

	if (AimingCameraTimeline)
		AimingCameraTimeline->PlayFromStart();
}

void APlayerCharacter::StopAiming()
{
	bIsAiming = false;
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;

	if (AimingCameraTimeline)
		AimingCameraTimeline->Reverse();
}

void APlayerCharacter::UpdateCameraTimeline(float TimelineValue) const
{
	const FVector CameraLocation = FMath::Lerp(DefaultCameraLocation, AimingCameraLocation, TimelineValue);
	CameraBoom->SocketOffset = CameraLocation;
}

void APlayerCharacter::HandleDeath()
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
		DisableInput(PlayerController);
	
	GetWorldTimerManager().SetTimer(RestartTimer, this, &APlayerCharacter::Respawn,1.f, false);

}

void APlayerCharacter::Respawn() const
{
	UGameplayStatics::OpenLevel(this, *GetWorld()->GetMapName());
}



