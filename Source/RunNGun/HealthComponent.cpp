// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	MaxHp = 10;
}

void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	CurrentHp = MaxHp;
}

void UHealthComponent::TakeDamage(int DamageAmount)
{
	CurrentHp = FMath::Clamp(CurrentHp-DamageAmount, 0.0f, MaxHp);
	if (IsDead())
	{
		OnDeath.Broadcast();
		GetOwner()->SetActorHiddenInGame(true);
		GetOwner()->SetActorEnableCollision(false);
	}
}

void UHealthComponent::Heal(float HealAmount)
{
	CurrentHp = FMath::Clamp(CurrentHp + HealAmount, 0.0f, MaxHp);
}

bool UHealthComponent::IsDead() const
{
	return CurrentHp <= 0.0f;
}



