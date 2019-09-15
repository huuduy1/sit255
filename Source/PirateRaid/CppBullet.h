// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CppBullet.generated.h"

UCLASS()
class PIRATERAID_API ACppBullet : public AActor
{
	GENERATED_BODY()

	// Sphere collision component
	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
	class USphereComponent* CollisionComp;

	// Projectile movement component 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	class UProjectileMovementComponent* ProjectileMovement;

public:	
	// Sets default values for this actor's properties
	ACppBullet();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called when projectile hits something
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	// Returns CollisionComp subobject 
	FORCEINLINE class USphereComponent* GetCollisionComp() const { return CollisionComp; }
	
	// Returns ProjectileMovement subobject 
	FORCEINLINE class UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }

	UPROPERTY()
	uint8 IsPlayerBullet;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Static Mesh
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* body;


};
