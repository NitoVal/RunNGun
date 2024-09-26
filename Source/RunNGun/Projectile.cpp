// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"

#include "HealthComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"


// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	ProjectileMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("ProjectileMesh");
	ProjectileMeshComponent->SetupAttachment(RootComponent);
	
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovementComponent");
	ProjectileMovementComponent->InitialSpeed = 5000.f;
	ProjectileMovementComponent->MaxSpeed = 10000.f;
	ProjectileMovementComponent->ProjectileGravityScale = 0.f;
}

void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
void AProjectile::NotifyActorBeginOverlap(AActor* OtherActor)
{
	if (OtherActor)
	{
		UHealthComponent* HealthComp = OtherActor->FindComponentByClass<UHealthComponent>();
		if (HealthComp)
		{
			HealthComp->TakeDamage(10);
		}
		Destroy();
	}
}


