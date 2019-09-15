// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerShipMovementComponent.h"

void UPlayerShipMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Null check
	if (!PawnOwner || !UpdatedComponent || ShouldSkipUpdate(DeltaTime))
	{
		return;
	}

	FVector MovementThisFrame = ConsumeInputVector() * DeltaTime;
	FRotator RotationThisFrame = UpdatedComponent->GetComponentRotation() + Rotation * DeltaTime;
	
	// Reset rotation
	Rotation = FRotator(0, 0, 0);

	if (MovementThisFrame.IsNearlyZero() && RotationThisFrame.IsNearlyZero())
	{
		return;
	}
	FHitResult Hit;
	SafeMoveUpdatedComponent(MovementThisFrame, RotationThisFrame, true, Hit);
}