// Fill out your copyright notice in the Description page of Project Settings.


#include "BowWeapon.h"

void UBowWeapon::Attack(FVector direction)
{
	FActorSpawnParameters spawnParams;
	spawnParams.Owner = GetOwner();
	spawnParams.Instigator = nullptr;
	spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	FVector shootingDirection = FVector(direction.X, direction.Y, 0);
	if (shootingDirection == FVector::ZeroVector) {
		shootingDirection = GetOwner()->GetActorForwardVector();
	}


	FVector spawnLocation = GetOwner()->GetActorLocation();
	FRotator spawnRotation = FRotator::ZeroRotator;

	AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(AProjectile::StaticClass(), spawnLocation, spawnRotation, spawnParams); // add offset to rotation if done at once somehow
	Projectile->SetDamage(weaponDamage);
	Projectile->SetDirection(shootingDirection);
	Projectile->SetSpeed(5.0f);
	Projectile->SetIfShotByPlayer(byPlayer);
	Projectile->SetupCollisions();
}

UBowWeapon::UBowWeapon()
{
	weaponDamage = 100;
	attackCooldown = 1.0f;
	attackRange = 1000.0f;
	onCooldown = false;
}
