// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LaunchPad.generated.h"

UCLASS()
class RUNNGUN_API ALaunchPad : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALaunchPad();

	UPROPERTY(EditAnywhere, Category="Launch Settings")
	FVector LaunchVector;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void NotifyActorBeginOverlap(AActor* OtherActor) override;
};
