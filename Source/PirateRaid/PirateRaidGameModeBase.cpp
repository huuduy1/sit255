// Fill out your copyright notice in the Description page of Project Settings.

#include "PirateRaidGameModeBase.h"
#include "Engine.h"



APirateRaidGameModeBase::APirateRaidGameModeBase()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bCanEverTick = true;

	NumberOfEnemyShipsDestroyed = 0;

	WindChangeTime = 1.0f;
	WindDirection = 0.0f;
	WindSpeed = 10.0f;
	WindChangeTimeKeeper = 0.0f;
	LoadConfig();
	ReloadConfig();
	if (this->numberOfEnemy > 100)
	{
		numberOfEnemy = 10;		
	}
	if (this->numberOfIslands > 10)
	{
		numberOfIslands = 2;		
	}
	if (this->numberOfEnemy < 1)
	{
		numberOfEnemy = 10;		
	}

	if (this->numberOfIslands < 1)
	{
		numberOfIslands = 2;		
	}
	SaveConfig();
	// insert config values from file
	configMap.Add(FString::Printf(TEXT("numberOfEnemy")), numberOfEnemy);
	configMap.Add(FString::Printf(TEXT("numberOfIslands")), numberOfIslands);
}

// Begin Play
void APirateRaidGameModeBase::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void APirateRaidGameModeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	WindChangeTimeKeeper += DeltaTime;
	if (WindChangeTimeKeeper >= WindChangeTime)
	{
		//time to calculate a new wind speed and direction
		ChangeWind();		
	}

	// If the desired number of enemies are destroyed, end the level
	if (NumberOfEnemyShipsDestroyed == NumberOfShipsDestroyedToWin)
	{
		TArray<AActor*> AllEnemyShips;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemyShip::StaticClass(), AllEnemyShips);
		for (AActor* EnemyShip : AllEnemyShips)
		{
			EnemyShip->Destroy();
		}
		UGameplayStatics::OpenLevel(GetWorld(), "Menu");
	}
}

// Changes the wind direction and speed at random intervals. Broadcasts the change.
void APirateRaidGameModeBase::ChangeWind()
{
	WindChangeTimeKeeper = 0.0f;
	WindDirection = FMath::RandRange(-179.0f, 180.0f);
	WindSpeed = FMath::RandRange(10.0f, 40.0f);
	WindChangeTime = FMath::RandRange(5.0f, 40.0f);

	//raise a wind change event here
	WindChange.Broadcast(WindDirection, WindSpeed);	
}