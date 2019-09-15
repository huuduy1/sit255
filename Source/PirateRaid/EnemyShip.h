// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Components/StaticMeshComponent.h"
#include "PlayerShipMovementComponent.h"
#include "CppBullet.h"
#include "EnemyShip.generated.h"


UCLASS()
class PIRATERAID_API AEnemyShip : public APawn
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemyShip();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Movement Component
	UPlayerShipMovementComponent* MovementComponent;

	FTimerHandle TimerHandle_EndRotateOnReceiveDamage;
	FTimerHandle TimerHandle_AutoAttack;
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Returns Movement Component
	virtual UPawnMovementComponent* GetMovementComponent() const override;

	// Ship's static mesh
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* ship;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 GotAttacked;
	// Ship's initial hit points
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ShipMaxHP = 10.0f;

	// Ship's current hit points
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ShipCurrentHP = 10.0f;
	
	// Ship's rotation speed modifier
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ShipRotationSpeed = 10.0f;

	// Ship's movement speed modifier
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ShipMovementSpeed = 10.f;

	// Ship's current movement speed
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ShipCurrentSpeed;

	// Maximum angle of sailing into the wind
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ShipMovementMaxAngle = 150.0f;

	// Wind direction 
	float WindDirection;

	// Wind speed 
	float WindSpeed;

	// Rotates the ship according to the passed value
	void RotateShip(float val);

	// Moves the ship in response to the wind 
	void MoveShipWithWind();

	// Method that reacts to wind change events generated in PirateShipGameModeBase
	UFUNCTION()
	void WindChanged(float direction, float speed);

	// Receives damage and ereduces hit points
	virtual void ReceivedDamage(float Point);

	UFUNCTION()
	void OnEndRotateByReceiveDamage();

	float RotateVal;

	/** Bullet class to spawn */
	UPROPERTY(Category = "Bullet", EditAnywhere, BlueprintReadOnly)
		TSubclassOf<class ACppBullet> BulletClass;

	// Ship's left and right cannon points
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gameplay")
		class UArrowComponent* MuzzleLeft_1;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gameplay")
		class UArrowComponent* MuzzleRight_1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float FireRate = 0.5f;

	void AutoAttack();

	/** Fires a projectile. */
	UFUNCTION(BlueprintCallable, Category = "OnFire")
		void OnFire(bool bOnLeft);
};
