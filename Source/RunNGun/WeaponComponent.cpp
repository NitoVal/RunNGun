// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponComponent.h"
#include "HealthComponent.h"

// Sets default values for this component's properties
UWeaponComponent::UWeaponComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	MaxAmmo = 100;
}

// Called when the game starts
void UWeaponComponent::BeginPlay()
{
	Super::BeginPlay();
	CurrentAmmo = MaxAmmo;
}

void UWeaponComponent::FireProjectile()
{
	if (CurrentAmmo <= 0)
		return;

	
	CurrentAmmo--;
}
void UWeaponComponent::FireHitscan()
{
	if (CurrentAmmo <= 0)
		return;
	
	FHitResult HitResult;

	// Perform the line trace
	if (PerformLineTrace(HitResult))
	{
		// Draw debug line for hit
		DrawDebugLine(GetWorld(), HitResult.TraceStart, HitResult.Location, FColor::Red, false, 1.0f, 0, 1.0f);

		// Apply damage to the hit actor
		AActor* HitActor = HitResult.GetActor();
		if (HitActor)
		{
			GEngine->AddOnScreenDebugMessage(-1,1,FColor::Red, HitActor->GetName());
			UHealthComponent* HealthComp = HitActor->FindComponentByClass<UHealthComponent>();
			if (HealthComp)
			{
				HealthComp->TakeDamage(1);
				FString hp = FString::SanitizeFloat(HealthComp->CurrentHp);
				FString mHp = FString::SanitizeFloat(HealthComp->MaxHp);
				GEngine->AddOnScreenDebugMessage(-1,1,FColor::Green, hp + "/" + mHp);
			}
		}
	}
	else
	{
		FVector TraceEnd = HitResult.TraceStart + HitResult.TraceEnd;
		DrawDebugLine(GetWorld(), HitResult.TraceStart, TraceEnd, FColor::Blue, false, 1.0f, 0, 1.0f);
	}
	CurrentAmmo--;
}


bool UWeaponComponent::PerformLineTrace(FHitResult& HitResult)
{
	FVector Start;
	FVector End;

	AActor* Owner = GetOwner();
	if (!Owner)
		return false;  // No valid owner

	// Check if the owner is controlled by a player or an AI
	AController* OwnerController = Owner->GetInstigatorController();
	if (OwnerController)
	{
		// For Player or AI, get the "viewpoint"
		FVector CameraLocation;
		FRotator CameraRotation;
		OwnerController->GetPlayerViewPoint(CameraLocation, CameraRotation);

		// Get forward direction and calculate trace end
		Start = CameraLocation;
		End = CameraLocation + CameraRotation.Vector() * 10000.f;
	}
	else
	{
		// For non-player AI characters (without controller viewpoint), use actor's forward direction
		Start = Owner->GetActorLocation();
		End = Start + Owner->GetActorForwardVector() * 10000.f;
	}

	// Collision parameters, ignore the actor itself
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(Owner);

	// Perform the line trace
	return GetWorld()->LineTraceSingleByChannel(HitResult,Start,End,ECC_Visibility,QueryParams);
}

