// Fill out your copyright notice in the Description page of Project Settings.


#include "Laser.h"
#include "HealthComponent.h"

// Sets default values
ALaser::ALaser()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	LaserMesh = CreateDefaultSubobject<UStaticMeshComponent>("LaserMesh");
	LaserMesh->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	RootComponent = LaserMesh;
}

// Called when the game starts or when spawned
void ALaser::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->GetTimerManager().SetTimer(LaserTimer, this,&ALaser::ActivateLaser, 2,true);
}

// Called every frame
void ALaser::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALaser::ActivateLaser()
{
	bIsActive = !bIsActive;
	
	if (bIsActive)
	{
		LaserMesh->SetVisibility(true);
		LaserMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
	else
	{
		LaserMesh->SetVisibility(false);
		LaserMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

