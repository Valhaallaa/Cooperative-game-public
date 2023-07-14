// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponComponent.h"

// Sets default values for this component's properties
UWeaponComponent::UWeaponComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	


	// ...
}


// Called when the game starts
void UWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


	if (onCooldown) {
		currentCooldown += DeltaTime;
		if (currentCooldown >= attackCooldown) {
			onCooldown = false;
			currentCooldown = 0;
		}
	}

	// ...
}

void UWeaponComponent::AttemptAttack(FVector direction)
{
	if (!onCooldown) {
		PlayerAttackFeedback();
		Attack(direction);
		onCooldown = true;
		
	}
}

void UWeaponComponent::SetDamage(int Value)
{
	weaponDamage = Value;
}

void UWeaponComponent::SetIsPlayer(bool Value)
{
	byPlayer = Value;
}

void UWeaponComponent::Attack(FVector direction)
{


}

