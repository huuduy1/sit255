// Fill out your copyright notice in the Description page of Project Settings.

#include "SailingShipState.h"
#include "PlayerShipPawn.h"
#include "AnchoredShipState.h"

// Rotates the ship in response to user input
void USailingShipState::RotateShip(class APlayerShipPawn* PlayerShip, float val)
{
	// Normal Rotation
	//UE_LOG(LogTemp, Warning, TEXT("In Sailing State!"));
	PlayerShip->RotateShip(val);
}

// Moves the ship in response to the wind
void USailingShipState::MoveShipWithWind(class APlayerShipPawn* PlayerShip)
{
	PlayerShip->MoveShipWithWind();
}

// Switches Ship State to Anchored
void USailingShipState::ToggleAnchor(class APlayerShipPawn* PlayerShip)
{
	PlayerShip->ShipState = NewObject<UAnchoredShipState>();
	PlayerShip->Anchored = true;
}