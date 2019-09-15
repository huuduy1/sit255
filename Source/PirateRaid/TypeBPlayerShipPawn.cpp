// Fill out your copyright notice in the Description page of Project Settings.

#include "TypeBPlayerShipPawn.h"

ATypeBPlayerShipPawn::ATypeBPlayerShipPawn(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ShipRotationSpeed = 25.0f;
	ShipMovementSpeed = 25.0f;
	ShipMaxHP = 50.0f;
	ShipCurrentHP = ShipMaxHP;
}