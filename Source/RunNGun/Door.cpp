// Fill out your copyright notice in the Description page of Project Settings.


#include "Door.h"

#include "PlayerCharacter.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"

// Sets default values
ADoor::ADoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMeshComponent"));
	RootComponent = DoorMesh;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->SetupAttachment(RootComponent);
	SphereComponent->SetSphereRadius(300.0f);
	
	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
	WidgetComponent->SetupAttachment(RootComponent);
	WidgetComponent->SetVisibility(false);
	
	minimumMoney = 5;
}
// Called when the game starts or when spawned
void ADoor::BeginPlay()
{
	Super::BeginPlay();
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ADoor::OnSphereOverlapBegin);
	SphereComponent->OnComponentEndOverlap.AddDynamic(this, &ADoor::OnSphereOverlapEnd);
}
void ADoor::Interact(APlayerCharacter* Player)
{
	if (Player && Player->Coins >= minimumMoney)
	{
		Player->Coins -= minimumMoney;
		Destroy();
	}
}

void ADoor::OnSphereOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA(APlayerCharacter::StaticClass()))
		WidgetComponent->SetVisibility(true);
}

void ADoor::OnSphereOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,int32 OtherBodyIndex)
{
	if (OtherActor->IsA(APlayerCharacter::StaticClass()))
		WidgetComponent->SetVisibility(false);
}



