// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameUserSettings.h"
#include "PirateRaidGameUserSettings.generated.h"

/**
 * 
 */
UCLASS()
class PIRATERAID_API UPirateRaidGameUserSettings : public UGameUserSettings
{
	GENERATED_BODY()
public:
	UPirateRaidGameUserSettings();

	UPROPERTY(Config)
	int numberOfEnemyShip;

	UPROPERTY(Config)
	FString test1_string;
};
