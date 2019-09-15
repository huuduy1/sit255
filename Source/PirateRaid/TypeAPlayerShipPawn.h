// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerShipPawn.h"
#include "TypeAPlayerShipPawn.generated.h"

/**
 * 
 */
UCLASS()
class PIRATERAID_API ATypeAPlayerShipPawn : public APlayerShipPawn
{
	GENERATED_BODY()
public:
	ATypeAPlayerShipPawn(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};
