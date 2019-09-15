// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerShipPawnOriginal.h"

// Sets default values
APlayerShipPawnOriginal::APlayerShipPawnOriginal()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create the static mesh body of the ship
	body = CreateDefaultSubobject<UStaticMeshComponent>("body");
	RootComponent = body;

	// Camera attached to the ship with a Spring Arm:
	springArm = CreateDefaultSubobject<USpringArmComponent>("springArm");
	camera = CreateDefaultSubobject<UCameraComponent>("camera");
	springArm->SetupAttachment(body);

	springArm->TargetArmLength = SpringArmLength;
	springArm->RelativeRotation = FRotator(-45.f, 0.f, 0.f);
	springArm->RelativeLocation = FVector(0, 30.0f, 0);
	camera->SetupAttachment(springArm);

	// Movement Component
	MovementComponent = CreateDefaultSubobject<UPlayerShipMovementComponent>("movementComponent");
	MovementComponent->UpdatedComponent = RootComponent;
}

// Called when the game starts or when spawned
void APlayerShipPawnOriginal::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlayerShipPawnOriginal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerShipPawnOriginal::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Bind Rotation keys A & D
	PlayerInputComponent->BindAxis("ShipRotateRight", this, &APlayerShipPawnOriginal::RotateRight);
}

// Called to move Actor to the right
void APlayerShipPawnOriginal::RotateRight(float val)
{
	if (val)
	{
		// Add rotation
		if (MovementComponent && (MovementComponent->UpdatedComponent == RootComponent))
		{
			MovementComponent->Rotation = FRotator(0.f, val * ShipRotationSpeed, 0.f);
		}
	}
}

UPawnMovementComponent* APlayerShipPawnOriginal::GetMovementComponent() const
{
	return MovementComponent;
}
