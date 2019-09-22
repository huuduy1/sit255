// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PawnMovementComponent.h"
#include "PlayerShipMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class PIRATERAID_API UPlayerShipMovementComponent : public UPawnMovementComponent
{
	GENERATED_BODY()
	
		// Tick component override
		virtual void TickComponent
		(
			float DeltaTime,
			enum ELevelTick TickType,
			FActorComponentTickFunction * ThisTickFunction
		) override;

public:

	// Ship rotation amount used each tick
	FRotator Rotation;

	void OnImpact(const FHitResult & Impact, const FVector & MoveDelta);

	FVector ComputeBounceResult(const FVector HitNormal, const FVector & MoveDelta);
};
