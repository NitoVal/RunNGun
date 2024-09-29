// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractionInterface.h"
#include "GameFramework/Actor.h"
#include "SlotMachine.generated.h"


class UWidgetComponent;
class USphereComponent;
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
	UMaterial* DefaultLineMaterial;

	UPROPERTY(EditAnywhere)
	UMaterial* WinLineMaterial;

	UPROPERTY(VisibleAnywhere, Category="Components")
	USphereComponent* SphereComponent;
	
	UPROPERTY(VisibleAnywhere, Category="Components")
	UWidgetComponent* WidgetComponent;
	
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

	virtual void Interact(APlayerCharacter* Player) override;
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
private:
	UFUNCTION()
	void OnSphereOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnSphereOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
};
