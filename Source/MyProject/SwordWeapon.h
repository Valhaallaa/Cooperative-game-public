// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "SwordWeapon.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API USwordWeapon : public UWeaponComponent
{
	GENERATED_BODY()

		virtual void Attack(FVector direction) override;
	
public:
	USwordWeapon();
};
