// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/GameUserSettings.h"
#include "EnemyShip.h"
#include "SpawnArea.h"

#include "PirateRaidGameModeBase.generated.h"

//delegate method declaration for all objects that are interested. eg Playership enemyship, HUD display
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnWindChange, float, direction, float, speed);
/**
 * 
 */
UCLASS(config=DefaultPirateRaidSetting)
class PIRATERAID_API APirateRaidGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	

public:
	APirateRaidGameModeBase();

	virtual void Tick(float DeltaTime) override;

	// Event to bbroadcast on wind change
	UPROPERTY(BlueprintAssignable, Category = "Event")
	FOnWindChange WindChange;

	//config item saved to DefaultPirateRaidSetting
	UPROPERTY(Config, BlueprintReadWrite)
	int numberOfEnemy = 10;//per spawn area

	//config item saved to DefaultPirateRaidSetting
	UPROPERTY(Config, BlueprintReadWrite)
	int numberOfIslands = 2;//per spawn area

	// a map for config items
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FString, int> configMap;

	// Tracks the number of enemy ships destroyed by the player
	UPROPERTY(BlueprintReadOnly, Category = "Enemy")
	int32 NumberOfEnemyShipsDestroyed;

	// Sets the number of ships to destroy to reach winning conditions
	UPROPERTY(BlueprintReadWrite, Category = "Enemy")
	int32 NumberOfShipsDestroyedToWin = 2;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called when the wind change timer runs out
	virtual void ChangeWind();

	//time in seconds until next wind direction change
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float WindChangeTime;

	//time in seconds since last wind change
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float WindChangeTimeKeeper;

	//wind direction 0 to 359
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float WindDirection;

	//wind speed
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float WindSpeed;

	// Enemy ship class for spawner
	UPROPERTY(Category = "Enemy", EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AEnemyShip>BP_EnemyShip;
};
