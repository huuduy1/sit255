// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ShipState.generated.h"

/**
 * 
 */
UCLASS()
class PIRATERAID_API UShipState : public UObject
{
	GENERATED_BODY()

public:

	// Rotates the ship in response to user input
	virtual void RotateShip(class APlayerShipPawn* PlayerShip, float val);

	// Moves the ship in response to the wind
	virtual void MoveShipWithWind(class APlayerShipPawn* PlayerShip);

	// Toggles Ship State
	virtual void ToggleAnchor(class APlayerShipPawn* PlayerShip);
};
