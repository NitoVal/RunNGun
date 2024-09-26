// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractionInterface.h"
#include "GameFramework/Actor.h"
#include "Door.generated.h"

class APlayerCharacter;
class UWidgetComponent;
class USphereComponent;
class UBoxComponent;

UCLASS()
class RUNNGUN_API ADoor : public AActor, public IInteractionInterface
{
	GENERATED_BODY()
	UPROPERTY(VisibleAnywhere, Category="Components")
	UStaticMeshComponent* DoorMesh;

	UPROPERTY(VisibleAnywhere, Category="Components")
	USphereComponent* SphereComponent;
	
	UPROPERTY(VisibleAnywhere, Category="Components")
	UWidgetComponent* WidgetComponent;
public:	
	// Sets default values for this actor's properties
	ADoor();
	
	UPROPERTY(EditAnywhere, Category="Requirements")
	int minimumMoney;
	
	virtual void Interact(APlayerCharacter* Player) override;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void OnSphereOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnSphereOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// void OpenDoor();

};
