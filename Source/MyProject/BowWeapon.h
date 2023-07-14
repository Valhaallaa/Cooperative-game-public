// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Projectile.h"
#include "DrawDebugHelpers.h"
#include "BowWeapon.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API UBowWeapon : public UWeaponComponent
{
	GENERATED_BODY()
	
		virtual void Attack(FVector direction) override;

public:
	UBowWeapon();
};
