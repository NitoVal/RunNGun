// Fill out your copyright notice in the Description page of Project Settings.


#include "Coin.h"

#include "Components/SphereComponent.h"

// Sets default values
ACoin::ACoin()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CoinMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CoinMesh"));
	RootComponent = CoinMesh;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	SphereComponent->SetupAttachment(RootComponent);
	SphereComponent->SetSphereRadius(50.f);
	SphereComponent->SetCollisionProfileName(TEXT("OverlapAllDynamic"));

	CoinValue = 1;
}

// Called when the game starts or when spawned
void ACoin::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACoin::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AddActorLocalRotation(FRotator(0,3,0));
}

