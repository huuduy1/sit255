// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ShipState.h"
#include "AnchoredShipState.generated.h"

/**
 * 
 */
UCLASS()
class PIRATERAID_API UAnchoredShipState : public UShipState
{
	GENERATED_BODY()

public:

	// Rotates the ship in response to user input
	virtual void RotateShip(class APlayerShipPawn* PlayerShip, float val) override;

	// Moves the ship in response to the wind
	virtual void MoveShipWithWind(class APlayerShipPawn* PlayerShip) override;

	// Switches Ship State to Sailing
	virtual void ToggleAnchor(class APlayerShipPawn* PlayerShip) override;
};
