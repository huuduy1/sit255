// Fill out your copyright notice in the Description page of Project Settings.

#include "TypeAPlayerShipPawn.h"

ATypeAPlayerShipPawn::ATypeAPlayerShipPawn(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ShipRotationSpeed = 5.0f;
	ShipMovementSpeed = 5.0f;
	ShipMaxHP = 1000.0f;
	ShipCurrentHP = ShipMaxHP;
}
