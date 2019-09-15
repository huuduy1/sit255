// Fill out your copyright notice in the Description page of Project Settings.

#include "AnchoredShipState.h"
#include "SailingShipState.h"
#include "PlayerShipPawn.h"

// Rotates the ship in response to user input
void UAnchoredShipState::RotateShip(class APlayerShipPawn* PlayerShip, float val)
{
	// Do not rotate ship if anchored
	// UE_LOG(LogTemp, Warning, TEXT("In Anchored State - Ship does not rotate."));
}

// Moves the ship in response to the wind
void UAnchoredShipState::MoveShipWithWind(class APlayerShipPawn* PlayerShip)
{
	// Do not move ship if anchored
	PlayerShip->ShipCurrentSpeed = 0;	
}

// Switches Ship State to Sailing
void UAnchoredShipState::ToggleAnchor(class APlayerShipPawn* PlayerShip)
{
	PlayerShip->ShipState = NewObject<USailingShipState>();
	PlayerShip->Anchored = false;
}