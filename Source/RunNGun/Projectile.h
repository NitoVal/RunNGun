// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

class UProjectileMovementComponent;

UCLASS()
class RUNNGUN_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectile();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	USceneComponent* DefaultSceneRootComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UStaticMeshComponent* ProjectileMeshComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UProjectileMovementComponent* ProjectileMovementComponent;
	
	void NotifyActorBeginOverlap(AActor* OtherActor) override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
