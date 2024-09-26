// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeath);
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RUNNGUN_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()
public:	
	// Sets default values for this component's properties
	UHealthComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float MaxHp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health")
	float CurrentHp;
	
	// Damage application function
	UFUNCTION(BlueprintCallable)
	void TakeDamage(int DamageAmount);

	// Heal the actor
	void Heal(float HealAmount);

	bool IsDead() const;

	// Event to broadcast on death
	UPROPERTY(BlueprintAssignable, Category = "Health")
	FOnDeath OnDeath;
protected:
	virtual void BeginPlay() override;

private:
};
