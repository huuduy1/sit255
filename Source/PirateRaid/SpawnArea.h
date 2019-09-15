// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyShip.h"
#include "PlayerShipPawn.h"
#include "GameFramework/Actor.h"
#include "SpawnArea.generated.h"

UCLASS()
class PIRATERAID_API ASpawnArea : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawnArea();

protected:
	// Called when the game starts or when spawned
	//virtual void BeginPlay() override;

public:	

	// Spawns the initial number of enemy ships
	UFUNCTION(BluePrintCallable)
	void SpawnAll(FVector location, FVector spawnerBounds,int numberToSpawn);
	
	//the blueprint we are going to spawn
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AActor>ActorToSpawn;

	//a string used to determine which config item applies to the spawnarea instance
	//Each instance can have a different class/qty. 
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString configItemSpawnQtyKey;

	
	
};
