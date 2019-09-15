// Fill out your copyright notice"GameFramework in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "PlayerShipMovementComponent.h"
#include "ShipState.h"
//#include "CppBullet.h"
#include "PlayerShipPawn.generated.h"

UCLASS()
class PIRATERAID_API APlayerShipPawn : public APawn
{
	GENERATED_BODY()

	// Declare a delegate in order to pass a parameter to OnFire function with BindAction
	DECLARE_DELEGATE_OneParam(FOnFireDelegate, bool);
	FOnFireDelegate onFireDelegate;

public:
	// Sets default values for this pawn's properties
	APlayerShipPawn(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

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

	// Returns Movement Component
	virtual UPawnMovementComponent* GetMovementComponent() const override;

	// Ship's static mesh
	UPROPERTY (EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* ship;

	// Spring arm component
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USpringArmComponent* springArm;

	// Camera attached to the player ship
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCameraComponent* camera;

	// Spring Arm length
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SpringArmLength = 3000.0f;	

	// Modifier for zoom sensitivity
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ZoomMultiplier = 100.0f;

	// Ship's initial hit points
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ShipMaxHP = 100.0f;

	// Ship's current hit points
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ShipCurrentHP = 100.0f;

	// Ship's rotation speed modifier
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ShipRotationSpeed = 10.0f;

	// Ship's movement speed modifier
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ShipMovementSpeed = 20.f;

	// Ship's current movement speed
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ShipCurrentSpeed;

	// Maximum angle of sailing into the wind
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ShipMovementMaxAngle = 150.0f;

	// Ship's left and right cannon points
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gameplay")
		class UArrowComponent* MuzzleLeft_1;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gameplay")
		class UArrowComponent* MuzzleRight_1;
	
	/** Bullet class to spawn */
	UPROPERTY(Category = "Bullet", EditAnywhere, BlueprintReadOnly)
		TSubclassOf<class ACppBullet> BulletClass;
	
	/** Sound to play each time we fire */
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	//	class USoundBase* FireSound;

	// State indicator for Blueprint HUD display
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "State Indicator")
		bool Anchored = true;

	// State Class variable
	UPROPERTY()
	class UShipState* ShipState;

	// Delegates ship rotation to the State object
	void HandleRotation(float val);

	// Rotates the ship in response to user input (controlled by ShipState)
	void RotateShip(float val);

	// Moves the ship in response to the wind (controlled by ShipState)
	void MoveShipWithWind();

	// Drops and raises the anchor, changing the Ship State
	void ToggleAnchor();

	// Zooms the camera in response to user input
	void ZoomCamera(float val);

	// Read-only properties providing data for Blueprint HUD:
	// Wind direction indicator
	UPROPERTY(BlueprintReadOnly)
	float WindDirection;

	// Wind speed indicator
	UPROPERTY(BlueprintReadOnly)
	float WindSpeed;

	// Ship direction (bearing) indicator
	UPROPERTY(BlueprintReadOnly)
	float ShipDirection;


	// Method that reacts to wind change events generated in PirateShipGameModeBase
	UFUNCTION()
	void WindChanged(float direction, float speed);

	/** Fires a projectile. */
	UFUNCTION(BlueprintCallable, Category = "OnFire")
	void OnFire(bool bOnLeft);

	// Receives damage and ereduces hit points
	virtual void ReceivedDamage(float Point);
};
