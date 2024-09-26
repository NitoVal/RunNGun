// Fill out your copyright notice in the Description page of Project Settings.


#include "LaunchPad.h"

#include "PlayerCharacter.h"

// Sets default values
ALaunchPad::ALaunchPad()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	LaunchVector = FVector(0, 0, 1000.0f);
}

// Called when the game starts or when spawned
void ALaunchPad::BeginPlay()
{
	Super::BeginPlay();
	
}

void ALaunchPad::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	if (OtherActor->IsA(APlayerCharacter::StaticClass()))
	{
		APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(OtherActor);
		if (PlayerCharacter)
			PlayerCharacter->LaunchCharacter(LaunchVector, false, true);
	}
}

