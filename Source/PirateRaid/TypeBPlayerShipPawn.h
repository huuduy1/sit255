// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerShipPawn.h"
#include "TypeBPlayerShipPawn.generated.h"

/**
 * 
 */
UCLASS()
class PIRATERAID_API ATypeBPlayerShipPawn : public APlayerShipPawn
{
	GENERATED_BODY()
public:
	ATypeBPlayerShipPawn(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};
