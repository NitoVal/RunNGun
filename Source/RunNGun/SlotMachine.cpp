// Fill out your copyright notice in the Description page of Project Settings.


#include "SlotMachine.h"

#include "NiagaraComponent.h"
#include "PlayerCharacter.h"
#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ASlotMachine::ASlotMachine()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	DefaultSceneRootComponent = CreateDefaultSubobject<USceneComponent>("DefaultSceneRoot");
	RootComponent = DefaultSceneRootComponent;
	
	Reels = CreateDefaultSubobject<USceneComponent>("Reels");
	Reels->SetupAttachment(RootComponent);
	
	ReelLeft = CreateDefaultSubobject<UStaticMeshComponent>("ReelLeft");
	ReelLeft->SetupAttachment(Reels);

	ReelMiddle = CreateDefaultSubobject<UStaticMeshComponent>("ReelMiddle");
	ReelMiddle->SetupAttachment(Reels);

	ReelRight = CreateDefaultSubobject<UStaticMeshComponent>("ReelRight");
	ReelRight->SetupAttachment(Reels);

	Lever = CreateDefaultSubobject<USceneComponent>("Lever");
	Lever->SetupAttachment(RootComponent);

	Line = CreateDefaultSubobject<UStaticMeshComponent>("Line");
	Line->SetupAttachment(RootComponent);

	WinVFX = CreateDefaultSubobject<UNiagaraComponent>("WinVFX");
	WinVFX->SetupAttachment(RootComponent);
	WinVFX->SetAutoActivate(false);

	StartAudioComponent = CreateDefaultSubobject<UAudioComponent>("StartSFX");
	StartAudioComponent->SetupAttachment(RootComponent);
	
	WinAudioComponent = CreateDefaultSubobject<UAudioComponent>("WinSFX");
	WinAudioComponent->SetupAttachment(RootComponent);

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->SetupAttachment(RootComponent);
	SphereComponent->SetSphereRadius(300.0f);
	
	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
	WidgetComponent->SetupAttachment(RootComponent);
	WidgetComponent->SetVisibility(false);
	
	Price = 5;
	bIsSpinning = false;
	
}

// Called when the game starts or when spawned
void ASlotMachine::BeginPlay()
{
	Super::BeginPlay();
	StartAudioComponent->Stop();
	WinAudioComponent->Stop();

	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ASlotMachine::OnSphereOverlapBegin);
	SphereComponent->OnComponentEndOverlap.AddDynamic(this, &ASlotMachine::OnSphereOverlapEnd);
}
// Called every frame
void ASlotMachine::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bIsSpinning)
	{
		LeverAngle = Lever->GetComponentRotation().Roll;
		LeverAngle = FMath::Max(LeverAngle - DeltaTime * 50,0);

		if (bIsLeftSpinning)
			ReelLeft->AddLocalRotation(FRotator(0,10,0));
		if (bIsMiddleSpinning)
			ReelMiddle->AddLocalRotation(FRotator(0,10,0));
		if (bIsRightSpinning)
			ReelRight->AddLocalRotation(FRotator(0,10,0));

		Lever->SetRelativeRotation(FRotator(0,0,LeverAngle));
	}
}

void ASlotMachine::Interact(APlayerCharacter* Player)
{
	WinVFX->Deactivate();
	
	if (Player->Coins >= Price && !bIsSpinning)
	{
		Line->SetMaterial(0,DefaultLineMaterial);
		StartAudioComponent->Play();
		WinAudioComponent->Stop();
		
		bIsSpinning = true;
		bIsLeftSpinning = true;
		bIsMiddleSpinning = true;
		bIsRightSpinning = true;

		Player->Coins -= Price;
		Lever->AddLocalRotation(FRotator(0,0,45));

		GetWorldTimerManager().SetTimer(ReelLeftTimer, this, &ASlotMachine::StopLeftReel, 1.f, false);
		GetWorldTimerManager().SetTimer(ReelMiddleTimer, this, &ASlotMachine::StopMiddleReel, 1.5f, false);
		GetWorldTimerManager().SetTimer(ReelRightTimer, this, &ASlotMachine::StopRightReel, 2.f, false);
		GetWorldTimerManager().SetTimer(SpinTimer, this, &ASlotMachine::StopMachine, 2.25f, false);

	}
}
int ASlotMachine::SpinReel(UStaticMeshComponent* Reel)
{
	int RandomInt = FMath::RandRange(0, 9);
	float LiteralFloat = 36.f;
	float Result = RandomInt * LiteralFloat;
	FRotator NewRotation = FRotator(Result, 90.f, 90.f);
	Reel->SetRelativeRotation(NewRotation);
	
	return RandomInt;
}

void ASlotMachine::StopLeftReel()
{
	bIsLeftSpinning = false;
	ResultLeft = SpinReel(ReelLeft);
}

void ASlotMachine::StopMiddleReel()
{
	bIsMiddleSpinning = false;
	ResultMiddle = SpinReel(ReelMiddle);
}

void ASlotMachine::StopRightReel()
{
	bIsRightSpinning = false;
	ResultRight = SpinReel(ReelRight);
}

void ASlotMachine::StopMachine()
{
	bIsSpinning = false;
	CheckResult();
}

void ASlotMachine::CheckResult() const
{
	int WinAmount = 0;
	
	if (ResultLeft == ResultMiddle && ResultMiddle == ResultRight)
		WinAmount = 100;
	else if (ResultLeft == ResultMiddle || ResultLeft == ResultRight || ResultMiddle == ResultRight)
		WinAmount = 5;

	if (WinAmount > 0)
	{
		WinVFX->Activate();
		WinAudioComponent->Play();
		Line->SetMaterial(0,WinLineMaterial);
		APlayerCharacter* Player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		if (Player)
			Player->Coins += WinAmount;
	}
}
void ASlotMachine::OnSphereOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA(APlayerCharacter::StaticClass()))
		WidgetComponent->SetVisibility(true);
}

void ASlotMachine::OnSphereOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,int32 OtherBodyIndex)
{
	if (OtherActor->IsA(APlayerCharacter::StaticClass()))
		WidgetComponent->SetVisibility(false);
}