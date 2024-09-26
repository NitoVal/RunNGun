// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

class UWeaponComponent;
class UNiagaraComponent;
class UHealthComponent;
class UTimelineComponent;
class IInteractionInterface;
class UPauseWidget;
class UPlayerWidget;
class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
UCLASS()
class RUNNGUN_API APlayerCharacter : public ACharacter
{
GENERATED_BODY()
	
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;
	
	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;
	
	/** Sprint Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SprintAction;
	
	/** Dash Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* DashAction;
	
	/**Interact Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* InteractAction;
	
	/** Pause Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* PauseAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* FireAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* AimAction;
public:
	// Sets default values for this character's properties
	APlayerCharacter();
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UHealthComponent* HealthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UWeaponComponent* WeaponComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UNiagaraComponent* DashNiagaraSystem;
	
	/** Returns CameraBoom subobject **/
	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	
	/** Returns FollowCamera subobject **/
	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Character | Currency")
	int Coins;

	UPROPERTY(VisibleAnywhere, Category="Character | Interaction", meta = (AllowPrivateAccess = "true"))
	TScriptInterface<IInteractionInterface> CurrentInteractable;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Character | Interaction", meta=(AllowPrivateAccess = "true"))
	bool bIsTargeting;
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default")
	bool bIsDashing;
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default")
	float DashCooldown;

	FTimerHandle DashVFXTimer;
	FTimerHandle DelayDash;
	FTimerHandle StopFiringTimer;
	FTimerHandle RestartTimer;
	
	void SetDashState();
	void DeactivateDashVFX() const;

protected:
	bool bIsAiming;
	
	UPROPERTY(VisibleAnywhere, Category="Character | Camera")
	FVector DefaultCameraLocation;
	
	UPROPERTY(VisibleAnywhere, Category="Character | Camera")
	FVector AimingCameraLocation;

	TObjectPtr<UTimelineComponent> AimingCameraTimeline;

	UPROPERTY(EditDefaultsOnly, Category ="Character | Aim Timeline")
	UCurveFloat* AimingCameraCurve;
	
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void ToggleSprint();
	void Interact();
	void InteractionCheck();
	void Dash();
	void Fire();
	void StopFiring();
	void Aim();
	void StopAiming();
	void Respawn() const;
	
	UFUNCTION()
	void HandleDeath();
	
	UFUNCTION()
	void UpdateCameraTimeline(float TimelineValue) const;
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	bool bIsSprinting;


};
