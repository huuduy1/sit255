// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerShipPawn.h"
#include "Engine.h"
#include "CppBullet.h"
#include "AnchoredShipState.h"
#include "PlayerShipMovementComponent.h"
#include "PirateRaidGameModeBase.h"


// Sets default values
APlayerShipPawn::APlayerShipPawn(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ship = CreateDefaultSubobject<UStaticMeshComponent>("playership");
	ship->OnComponentHit.AddDynamic(this, &APlayerShipPawn::OnHit);
	RootComponent = ship;

	springArm = CreateDefaultSubobject<USpringArmComponent>("springArm");
	camera = CreateDefaultSubobject<UCameraComponent>("camera");
	springArm->AttachTo(ship);
	camera->AttachTo(springArm);
	springArm->TargetArmLength = SpringArmLength;
	springArm->SocketOffset = FVector(0.0f, 300.0f, 200.0f);
	springArm->RelativeRotation = FRotator(-30.0f, 10.0f, 0.0f);
	
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

	// Set initial state to anchored
	ShipState = CreateDefaultSubobject<UAnchoredShipState>("state");
}

// Called when the game starts or when spawned
void APlayerShipPawn::BeginPlay()
{
	Super::BeginPlay();
	//subscribe to wind change events here. 
	//The method that listens will need to calculate and set the MovementSpeed based on gamemode variables WindSpeed, WindDirection. 
	APirateRaidGameModeBase* gmb = Cast<APirateRaidGameModeBase>(GetWorld()->GetAuthGameMode());
	gmb->WindChange.AddDynamic(this, &APlayerShipPawn::WindChanged);

	// TODO - CHange this when anchor is implemented
	ShipCurrentSpeed = 0;
		
}

// Called every frame
void APlayerShipPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (ShipState != NULL && ShipState->GetName() !="None")
	{
		// Delegate ship movement to the State object:
		ShipState->MoveShipWithWind(this);
	}
}

// Called to bind functionality to input
void APlayerShipPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{	
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("ShipRotate", this, &APlayerShipPawn::HandleRotation);
	PlayerInputComponent->BindAxis("ZoomCamera", this, &APlayerShipPawn::ZoomCamera);
	PlayerInputComponent->BindAction<FOnFireDelegate>("FireLeft", IE_Pressed, this, &APlayerShipPawn::OnFire, true);
	PlayerInputComponent->BindAction<FOnFireDelegate>("FireRight", IE_Pressed, this, &APlayerShipPawn::OnFire, false);
	PlayerInputComponent->BindAction("ToggleAnchor", IE_Pressed, this, &APlayerShipPawn::ToggleAnchor);
}

//allows the bound mousewheel input to zoom camera 
void APlayerShipPawn::ZoomCamera(float val)
{
	if (val) {
		SpringArmLength = FMath::Clamp(SpringArmLength + val * ZoomMultiplier,500.0f,10000.0f);
		springArm->TargetArmLength = SpringArmLength;
	}
}

// Delegates ship rotation to the State object
void APlayerShipPawn::HandleRotation(float val)
{
	if (ShipState != NULL && ShipState->GetName() != "None")
	{
		ShipState->RotateShip(this, val);
	}
}

// Moves the ship in response to the wind (controlled by ShipState)
void APlayerShipPawn::MoveShipWithWind()
{
	FRotator ShipRotation = GetActorRotation();
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
		MovementComponent->AddInputVector(GetActorForwardVector() * ShipCurrentSpeed);
	}
	// Show ship rotation in HUD
	ShipDirection = ShipRotation.Yaw;
}

// Rotates the ship in response to user input (controlled by ShipState)
void APlayerShipPawn::RotateShip(float val)
{
	if (val)
	{
		if (MovementComponent && MovementComponent->UpdatedComponent == RootComponent)
		{
			MovementComponent->Rotation = FRotator(0.0f, val * ShipRotationSpeed, 0.0f);
		}
	}
}

// Drops and raises the anchor, changing the Ship State
void APlayerShipPawn::ToggleAnchor()
{
	if (ShipState != NULL && ShipState->GetName() != "None")
	{
		ShipState->ToggleAnchor(this);
	}
}

// Returns Movement Component
UPlayerShipMovementComponent* APlayerShipPawn::GetMovementComponent() const
{
	return MovementComponent;
}

// Use the wind direction, wind speed and the ships current bearing to calculate the ship speed
void APlayerShipPawn::WindChanged(float windDirection, float windSpeed)
{
	WindDirection = windDirection;
	WindSpeed = windSpeed;

}

// Fires the cannons on either port or starboard side, determined by the parameter
void APlayerShipPawn::OnFire(bool bOnLeft)
{
	// Try and fire a projectile
	// Note: The BulletClass are set in the derived blueprint asset named BP_PlayerShipPawn 
	// to avoid direct content references in C++.
	if (BulletClass != NULL)
	{
		const UWorld* World = GetWorld();
		if (World != NULL)
		{
			FTransform tf = (bOnLeft)? MuzzleLeft_1->GetComponentTransform(): MuzzleRight_1->GetComponentTransform();
			FRotator SpawnRotation = tf.GetRotation().Rotator();
			FVector SpawnLocation = tf.GetLocation();

			//Set Spawn Collision Handling Override
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.Owner = this;
			ActorSpawnParams.Instigator = this;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			// spawn the projectile at the muzzle
			ACppBullet* bullet = GetWorld()->SpawnActor<ACppBullet>(BulletClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
		}
	}

	//// try and play the sound if specified
	//if (FireSound != NULL)
	//{
	//	UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
	//}
}

void APlayerShipPawn::ReceivedDamage(float Point)
{
	if (ShipCurrentHP > 0)
	{
		ShipCurrentHP -= Point;
		UE_LOG(LogTemp, Log, TEXT("%s received %f damage, remaining HP: %f"), *GetName(), Point, ShipCurrentHP);
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Yellow, FString::Printf(TEXT("%s received %f damage, remaining HP: %f"), *GetName(), Point, ShipCurrentHP));
		}		
	}

	if(ShipCurrentHP <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT(" Game Over !!"));
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(2, 5.f, FColor::Red, FString::Printf(TEXT("Game Over !!")));
		}		
	}
}

void APlayerShipPawn::OnHit(UPrimitiveComponent * HitComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, FVector NormalImpulse, const FHitResult & Hit)
{
	//if(OtherActor->GetName() == "BP_I"
	
	if (HitComp->GetCollisionProfileName().ToString() == "BlockAllDynamic") {
		FVector MoveDelta = GetActorForwardVector() * ShipCurrentSpeed;
		GetMovementComponent()->OnImpact(Hit, MoveDelta);
		ShipCurrentSpeed = ShipCurrentSpeed * 0.3f;
		ReceivedDamage(10.0f);		
	};
	UE_LOG(LogTemp, Warning, TEXT(" Hit Something !!  %s"), *HitComp->GetCollisionProfileName().ToString());	
}

