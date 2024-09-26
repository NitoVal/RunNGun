// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractionInterface.h"
#include "GameFramework/Actor.h"
#include "Switch.generated.h"

UCLASS()
class RUNNGUN_API ASwitch : public AActor, public IInteractionInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASwitch();

	void Interact(APlayerCharacter* Player) override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="State")
	bool bIsActive; 
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
