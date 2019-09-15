// Fill out your copyright notice in the Description page of Project Settings.

#include "CppBullet.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "EnemyShip.h"
#include "PlayerShipPawn.h"
// Sets default values
ACppBullet::ACppBullet()
{
	// Create the static mesh for the bullet
	body = CreateDefaultSubobject<UStaticMeshComponent>("body");

	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(20.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &ACppBullet::OnHit);		// set up a notification for when this component hits something blocking
																							// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComp;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 2000.0f;
	ProjectileMovement->MaxSpeed = 2000.0f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;

	// Die after 3 seconds by default
	InitialLifeSpan = 3.f;
	IsPlayerBullet = true;
}

// Called when the game starts or when spawned
void ACppBullet::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACppBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Causes damage on impact and despawns the bullet
void ACppBullet::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	UE_LOG(LogTemp, Log, TEXT("OnHit %s"), *GetNameSafe(OtherActor));
	ProjectileMovement->StopMovementImmediately();
	if (IsPlayerBullet){
		if (AEnemyShip *EnemyShip = Cast<AEnemyShip>(OtherActor)) {
			EnemyShip->ReceivedDamage(1.0f);
		}
	}else{		
		if (APlayerShipPawn *PlayerShip = Cast<APlayerShipPawn>(OtherActor)) {
			PlayerShip->ReceivedDamage(1.0f);			
		}		
	}
	Destroy();

}