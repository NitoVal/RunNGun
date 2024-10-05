// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Laser.generated.h"

UCLASS()
class RUNNGUN_API ALaser : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALaser();

	UPROPERTY(EditAnywhere, BlueprintReadOnly , Category="Components")
	UStaticMeshComponent* LaserMesh;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(VisibleAnywhere, Category="Laser | State")
	bool bIsActive;

	UPROPERTY(EditAnywhere, Category="Laser | Timer Settings")
	bool bIsTimed;

	UPROPERTY(EditAnywhere, Category="Laser | Timer Settings")
	float Seconds;
	
	virtual void Tick(float DeltaTime) override;

	FTimerHandle LaserTimer;

	void ActivateLaser();

};
