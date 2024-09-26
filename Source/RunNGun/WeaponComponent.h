// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RUNNGUN_API UWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWeaponComponent();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Ammo")
	int MaxAmmo;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Ammo")
	int CurrentAmmo;
	
	void FireHitscan();
	void FireProjectile();
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	// Function to handle the line trace logic
	bool PerformLineTrace(FHitResult& HitResult);
};
