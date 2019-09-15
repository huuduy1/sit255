// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "PlayerShipMovementComponent.h"
#include "PlayerShipPawnOriginal.generated.h"

UCLASS()
class PIRATERAID_API APlayerShipPawnOriginal : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APlayerShipPawnOriginal();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Movement Component
	UPlayerShipMovementComponent* MovementComponent;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Static mesh - body of the player ship
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* body;

	// Spring Arm to hold Camera
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USpringArmComponent* springArm;

	// Camera 
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UCameraComponent* camera;

	// Spring Arm length
	UPROPERTY(Category = "Length", EditAnywhere, BlueprintReadWrite)
		float SpringArmLength = 3000.0f;	
	
	// Ship rotation speed
	UPROPERTY(Category = "Ship properties", EditAnywhere, BlueprintReadWrite)
		float ShipRotationSpeed = 10.0f;

	// Ship movement control - Rotate Left/Right
	void RotateRight(float val);

	// Get Movement Component pointer
	virtual UPawnMovementComponent* GetMovementComponent() const override;
};
