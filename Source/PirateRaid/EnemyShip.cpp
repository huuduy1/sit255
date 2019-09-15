// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyShip.h"
#include "Engine.h"
#include "PirateRaidGameModeBase.h"


// Sets default values
AEnemyShip::AEnemyShip()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ship = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EnemyShip"));
	RootComponent = ship;
	ship->bHiddenInGame = false;
	ship->bVisible = true;
	
	MovementComponent = CreateDefaultSubobject<UPlayerShipMovementComponent>("movementComponent");
	MovementComponent->UpdatedComponent = RootComponent;

	// MuzzleLeft_1 - Left cannon
	MuzzleLeft_1 = CreateDefaultSubobject<UArrowComponent>(TEXT("MuzzleLeft_1"));
	FRotator rotLeft_1 = FRotator(0.f, -90.0f, 0.0f);
	MuzzleLeft_1->SetWorldRotation(rotLeft_1);
	MuzzleLeft_1->SetupAttachment(RootComponent);

	// MuzzleRight_1 - Right cannon
	MuzzleRight_1 = CreateDefaultSubobject<UArrowComponent>(TEXT("MuzzleRight_1"));
	FRotator rotRight_1 = FRotator(0.f, 90.0f, 0.0f);
	MuzzleRight_1->SetWorldRotation(rotRight_1);
	MuzzleRight_1->SetupAttachment(RootComponent);
}


// Called when the game starts or when spawned
void AEnemyShip::BeginPlay()
{
	Super::BeginPlay();

	APirateRaidGameModeBase* gmb = Cast<APirateRaidGameModeBase>(GetWorld()->GetAuthGameMode());
	gmb->WindChange.AddDynamic(this, &AEnemyShip::WindChanged);

	// TODO - Change this when anchor is implemented
	ShipCurrentSpeed = 0;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_AutoAttack, this, &AEnemyShip::AutoAttack, FireRate, true);
}

// Called every frame
void AEnemyShip::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Move the ship
	MoveShipWithWind();
	if (GotAttacked)
	{
		RotateShip(RotateVal);

	}	
}

// Moves the ship in response to the wind (controlled by ShipState)
void AEnemyShip::MoveShipWithWind()
{
	FRotator ShipRotation = GetActorRotation();
	ShipRotation.Yaw += 90.0f;
	float DeltaWindRotation = FMath::Abs(ShipRotation.Yaw - WindDirection);
	if (DeltaWindRotation > 180)
	{
		DeltaWindRotation = 360 - DeltaWindRotation;
	}

	// Determine the ship speed based on wind direction and strength
	if (DeltaWindRotation <= ShipMovementMaxAngle)
	{
		ShipCurrentSpeed = (1 - DeltaWindRotation / ShipMovementMaxAngle) * WindSpeed * ShipMovementSpeed;
	}
	else ShipCurrentSpeed = 0;

	// Add input vector
	if (MovementComponent && (MovementComponent->UpdatedComponent == RootComponent))
	{
		MovementComponent->AddInputVector(GetActorRightVector() * ShipCurrentSpeed);
	}
}

// Rotates the ship
void AEnemyShip::RotateShip(float val)
{
	if (val)
	{
		if (MovementComponent && MovementComponent->UpdatedComponent == RootComponent)
		{
			MovementComponent->Rotation = FRotator(0.0f, val * ShipRotationSpeed, 0.0f);
		}
	}
}

// Returns Movement Component
UPawnMovementComponent* AEnemyShip::GetMovementComponent() const
{
	return MovementComponent;
}

// Use the wind direction, wind speed and the ships current bearing to calculate the ship speed
void AEnemyShip::WindChanged(float windDirection, float windSpeed)
{
	WindDirection = windDirection;
	WindSpeed = windSpeed;
}

// Receives damage on impact and reduces HPs
void AEnemyShip::ReceivedDamage(float Point)
{
	if (ShipCurrentHP > 0)
	{
		ShipCurrentHP--;
		UE_LOG(LogTemp, Log, TEXT("%s received %f damage, remaining HP: %f"), *GetName(), Point, ShipCurrentHP);
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Yellow, FString::Printf(TEXT("%s received %f damage, remaining HP: %f"), *GetName(), Point, ShipCurrentHP));
		}

		// Change direction & start auto fire
		if (!GotAttacked) {
			GotAttacked = true;
			RotateVal = FMath::FRandRange(-5.f, 5.f);

			GetWorld()->GetTimerManager().SetTimer(TimerHandle_EndRotateOnReceiveDamage, this, &AEnemyShip::OnEndRotateByReceiveDamage, 1, false, 2);			
			UE_LOG(LogTemp, Log, TEXT("Begin RotateOnReceiveDamage %f"), RotateVal);

			GetWorld()->GetTimerManager().SetTimer(TimerHandle_AutoAttack, this, &AEnemyShip::AutoAttack, FireRate, true);
		}			
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("%s has been destroyed"), *GetName());
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(2, 5.f, FColor::Red, FString::Printf(TEXT("%s has been destroyed"), *GetName()));
		}

		APirateRaidGameModeBase* Game = Cast<APirateRaidGameModeBase>(GetWorld()->GetAuthGameMode());
		Game->NumberOfEnemyShipsDestroyed++;
		this->Destroy();
	}
}

void AEnemyShip::OnEndRotateByReceiveDamage()
{
	UE_LOG(LogTemp, Log, TEXT("End RotateOnReceiveDamage"));
	GotAttacked = false;
	RotateVal = 0.f;	
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle_AutoAttack);
}

void AEnemyShip::AutoAttack()
{
	OnFire(true);
	OnFire(false);
}

void AEnemyShip::OnFire(bool bOnLeft)
{
	// Try and fire a projectile
	// Note: The BulletClass are set in the derived blueprint asset named BP_PlayerShipPawn 
	// to avoid direct content references in C++.
	//UE_LOG(LogTemp, Log, TEXT("AEnemyShip OnFire"));
	if (BulletClass != NULL)
	{
		const UWorld* World = GetWorld();
		if (World != NULL)
		{
			FTransform tf = (bOnLeft) ? MuzzleLeft_1->GetComponentTransform() : MuzzleRight_1->GetComponentTransform();
			FRotator SpawnRotation = tf.GetRotation().Rotator();
			FVector SpawnLocation = tf.GetLocation();

			//Set Spawn Collision Handling Override
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.Owner = this;
			ActorSpawnParams.Instigator = this;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			// spawn the projectile at the muzzle
			ACppBullet* bullet = GetWorld()->SpawnActor<ACppBullet>(BulletClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
			bullet->IsPlayerBullet = false;
		}
	}

	//// try and play the sound if specified
	//if (FireSound != NULL)
	//{
	//	UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
	//}
}
