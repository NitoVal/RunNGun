// Fill out your copyright notice in the Description page of Project Settings.
#include "GravityPad.h"
#include "PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"


// Sets default values
AGravityPad::AGravityPad()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GravityDirection = 1.f;
}

// Called when the game starts or when spawned
void AGravityPad::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGravityPad::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGravityPad::NotifyActorBeginOverlap(AActor* OtherActor)
{
	if (OtherActor->IsA(APlayerCharacter::StaticClass()))
	{
		APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(OtherActor);
		if (PlayerCharacter)
		{
			PlayerCharacter->GetCharacterMovement()->SetGravityDirection(FVector(0,0,GravityDirection));
			//PlayerCharacter->GetFollowCamera()->AddLocalRotation(FRotator(0,0,180));
		}
	}
}

