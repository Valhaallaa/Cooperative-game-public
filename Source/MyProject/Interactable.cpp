// Fill out your copyright notice in the Description page of Project Settings.

#include "Interactable.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"

// Sets default values
AInteractable::AInteractable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}


// Called when the game starts or when spawned
void AInteractable::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AInteractable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (isCrank)
	{
		if (!crankComplete && crankValue > 0)
		{
			crankValue -= crankDecreaseValue;
			WhenNotInteracted();
		}
		else if (isCrankRepeatable) {
			crankComplete = false;
			crankValue -= crankDecreaseValue;
			WhenNotInteracted();
		}

		if (crankValue >= 100.0f)
		{
			crankComplete = true;
			//activatedObject->interactsActive++;
			ActivateLoop();
		}
		//FString FloatAsString = FString::SanitizeFloat(crankValue);
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Crank Value"), FloatAsString));

	}
}

void AInteractable::OnInteract()
{
	/*
	for (int i = 0; i < activatedObjects.Num(); i++)
	{

		AActivatable* objectToActivate = activatedObjects[i];
		objectToActivate.OnActivate();
	}
	*/


	if (isToggle && isToggled) //lever true
	{
		isToggled = false;
		DeActivateLoop();
	}
	else if (isToggle && !isToggled) //lever false
	{
		//activatedObject->interactsActive++;
		ActivateLoop();
		isToggled = true;
	}

	if (!isToggle &&  !isTimed && !isCrank) // pp true
	{
		//activatedObject->interactsActive++;
		ActivateLoop();

	}

	if (isTimed ) // button true
	{
		//activatedObject->interactsActive++;
		ActivateLoop();
		//activatedObject->isActivated = true;
		FTimerHandle activeTimer;
		//GetWorldTimerManager().SetTimer(activeTimer, this, &AInteractable::StopInteract, 1.0f, false, 2.0f);
		GetWorldTimerManager().SetTimer(activeTimer, this, &AInteractable::DeActivateLoop, 1.0f, false, 2.0f);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("TIMER"));
	}

	if (isCrank)
	{
		crankValue += 7.5f;
	}

	WhenInteracted();


}

void AInteractable::ActivateLoop() {

	if (activatedObjectArray.Num() > 0) {
		for (int i = 0; i < activatedObjectArray.Num(); i++)
		{
			
			if (activatedObjectArray[i] != nullptr) {
				//AActivatable* obj = Cast<AActivatable>(activatedObjectArray[i]);

				activatedObjectArray[i]->interactsActive++;
				activatedObjectArray[i]->OnActivate();
				//activatedObjectArray[i]->isActivated = true;
			}
		}
	}
	if (isOneUse) {
		activatedObjectArray.Empty();
	}
}

void AInteractable::DeActivateLoop() {

	if (activatedObjectArray.Num() > 0) {
		for (int i = 0; i < activatedObjectArray.Num(); i++)
		{

			if (activatedObjectArray[i] != nullptr) {
				//AActivatable* obj = Cast<AActivatable>(activatedObjectArray[i]);

				activatedObjectArray[i]->interactsActive--;
				activatedObjectArray[i]->StopActivate();
				//activatedObjectArray[i]->isActivated = false;
			}
		}
	}
}


void AInteractable::StopInteract()
{
	/*
	for (int i = 0; i < activatedObjects.Num(); i++)
	{

		AActivatable* objectToActivate = activatedObjects[i];
		objectToActivate->StopActivate();
	}
	*/
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("STOP"));
	activatedObject->interactsActive--;
	activatedObject->StopActivate();
	activatedObject->isActivated = false;

	//if (isToggled)
		//isToggled = false;
}

