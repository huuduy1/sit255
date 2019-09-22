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


void UPlayerShipMovementComponent::OnImpact(const FHitResult & Impact, const FVector & MoveDelta)
{
	if (!MoveDelta.IsNearlyZero()) {
		FHitResult Hit2(1.f);			
		FVector ImpactDir = ComputeBounceResult(Impact.Normal.GetSafeNormal2D(), MoveDelta).GetSafeNormal2D();
		SafeMoveUpdatedComponent(FVector::ZeroVector, ImpactDir.Rotation(), true, Hit2);				
	}
}

FVector UPlayerShipMovementComponent::ComputeBounceResult(const FVector HitNormal, const FVector & MoveDelta)
{
	FVector TempVelocity = MoveDelta;
	const FVector Normal = ConstrainNormalToPlane(HitNormal);
	const float VDotNormal = (TempVelocity | Normal);

	// Only if velocity is opposed by normal or parallel
	if (VDotNormal <= 0.f)
	{
		// Project velocity onto normal in reflected direction.
		const FVector ProjectedNormal = Normal * -VDotNormal;

		// Point velocity in direction parallel to surface
		TempVelocity += ProjectedNormal;

		// Only tangential velocity should be affected by friction.
		const float ScaledFriction = 0.2f;
		TempVelocity *= FMath::Clamp(1.f - ScaledFriction, 0.f, 1.f);

		// Coefficient of restitution only applies perpendicular to impact.
		TempVelocity += (ProjectedNormal * 0.5f);
	}

	return TempVelocity;
}
