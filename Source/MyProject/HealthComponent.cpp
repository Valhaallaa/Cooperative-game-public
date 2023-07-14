// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"
#include "MyProjectCharacter.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	currentHealth = maxHealth;
	canTakeDamage = true;
	currentIFrameDuration = 0;
	iFrames = false;
	// ...
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


	if (iFrames) {
		currentIFrameDuration += DeltaTime;
		canTakeDamage = false;
		if (currentIFrameDuration >= iFrameDuration) {
			canTakeDamage = true;
			iFrames = false;
			currentIFrameDuration = 0;
		}
	}
	// ...
}

void UHealthComponent::SetMaxHealth(int Value)
{
	maxHealth = Value;

	currentHealth = maxHealth;
}

void UHealthComponent::SetCurrentHealth(int Value)
{
	currentHealth = Value;
	if (currentHealth > maxHealth)
		currentHealth = maxHealth;
}

void UHealthComponent::TakeDamage(int Value)
{
	int storedDamage = Value;
	if (canTakeDamage && storedDamage > 0) {

		iFrames = true;
		
		currentHealth -= storedDamage;
		
		

		
		AMyProjectCharacter* Char = Cast<AMyProjectCharacter>(GetOwner());
			if (Char)
				Char->PlayerDamageFeedback();
		}
		if (currentHealth <= 0)
			Death();
	
}

void UHealthComponent::IncreaseCurrentHealth(int Value)
{
	currentHealth += Value;
	if (currentHealth > maxHealth)
		currentHealth = maxHealth;
}

void UHealthComponent::Death()
{
	GetOwner()->Destroy();
}
int UHealthComponent::GetCurrentHealth()
{
	return currentHealth;
}

int UHealthComponent::GetMaxHealth()
{
	return maxHealth;
}

void UHealthComponent::SetTakeDamage(bool Value)
{
	canTakeDamage = Value;
}

bool UHealthComponent::GetCanTakeDamage() {
	return canTakeDamage;
}




