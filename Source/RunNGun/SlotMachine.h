// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractionInterface.h"
#include "GameFramework/Actor.h"
#include "SlotMachine.generated.h"


class UNiagaraComponent;
class USoundBase;
UCLASS()
class RUNNGUN_API ASlotMachine : public AActor, public IInteractionInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASlotMachine();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Components")
	USceneComponent* DefaultSceneRootComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Components")
	USceneComponent* Reels;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Components")
	UStaticMeshComponent* ReelLeft;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Components")
	UStaticMeshComponent* ReelMiddle;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Components")
	UStaticMeshComponent* ReelRight;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Components")
	USceneComponent* Lever;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Components")
	UStaticMeshComponent* Line;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Components")
	UNiagaraComponent* WinVFX;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Components")
	UAudioComponent* StartAudioComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Components")
	UAudioComponent* WinAudioComponent;

	UPROPERTY(EditAnywhere)
	class UMaterial* DefaultLineMaterial;

	UPROPERTY(EditAnywhere)
	class UMaterial* WinLineMaterial;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Price")
	int Price;
	
	bool bIsSpinning;

	FTimerHandle ReelLeftTimer;
	FTimerHandle ReelMiddleTimer;
	FTimerHandle ReelRightTimer;
	FTimerHandle SpinTimer;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	int ReelIndex;
	int ResultLeft;
	int ResultMiddle;
	int ResultRight;
	float LeverAngle;

	bool bIsLeftSpinning;
	bool bIsMiddleSpinning;
	bool bIsRightSpinning;
	
	void Interact(APlayerCharacter* Player) override;
	void StopLeftReel();
	void StopMiddleReel();
	void StopRightReel();
	
	void CheckResult() const;
	UFUNCTION()
	int SpinReel(UStaticMeshComponent* Reel);
	
	void StopMachine();
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
};
