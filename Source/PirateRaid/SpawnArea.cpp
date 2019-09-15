// Fill out your copyright notice in the Description page of Project Settings.

#include "SpawnArea.h"
#include "EnemyShip.h"
#include "PlayerShipPawn.h"
#include "PirateRaidGameModeBase.h"
#include "Engine.h"

// Sets default values
ASpawnArea::ASpawnArea()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}


/*
Spawns the actor that is assigned to the blueprint in a random location within the blueprint defined bounds.
The number of items to spawn is sent at runtime from the gamemode persistent parameters associated with this spawnarea (ie numberOfIslands or numberOfEnemyShips)

 Called when the game starts or when spawned
 */
void ASpawnArea::SpawnAll(FVector location, FVector spawnerBounds, int numberToSpawn)
{
	
	UWorld* TheWorld = GetWorld();
	if (ActorToSpawn && numberToSpawn < 100)
	{	
		
		for(int i=0;i< numberToSpawn;i++)
		{
			
			float randX = FMath::RandRange(spawnerBounds.X*-1.0f, spawnerBounds.X);
			float randY = FMath::RandRange(spawnerBounds.Y*-1.0f, spawnerBounds.Y);
			float randZ = FMath::RandRange(spawnerBounds.Z*-0.01f, spawnerBounds.Z);
			
			FVector randomLocation = location.operator+(FVector(randX, randY, randZ));
			FRotator randomRotation = FRotator(0.0f, FMath::RandRange(-179.0f, 179.0f), 0.0f);
			AActor* toSpawn = TheWorld->SpawnActor<AActor>(ActorToSpawn, randomLocation, randomRotation);

		}
		
	}
}

