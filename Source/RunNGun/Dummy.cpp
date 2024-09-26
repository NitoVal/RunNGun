// Fill out your copyright notice in the Description page of Project Settings.


#include "Dummy.h"

#include "HealthComponent.h"
#include "Components/CapsuleComponent.h"

// Sets default values
ADummy::ADummy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>("CapsuleComponent");
	CapsuleComponent->InitCapsuleSize(55.f, 96.f);  // Set the size of the capsule
	CapsuleComponent->SetCollisionProfileName(TEXT("Pawn"));  // Set collision profile
	RootComponent = CapsuleComponent;
	
	HealthComponent = CreateDefaultSubobject<UHealthComponent>("HealthComponent");
}

// Called when the game starts or when spawned
void ADummy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADummy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

