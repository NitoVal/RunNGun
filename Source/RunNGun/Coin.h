// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Coin.generated.h"

class USphereComponent;

UCLASS()
class RUNNGUN_API ACoin : public AActor
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere, Category="Components")
	UStaticMeshComponent* CoinMesh;
	
	UPROPERTY(VisibleAnywhere, Category="Components")
	USphereComponent* SphereComponent;
	
public:	
	// Sets default values for this actor's properties
	ACoin();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category="Value")
	int32 CoinValue;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	

};
