// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Activatable.h"
#include "Interactable.generated.h"

UCLASS()
class MYPROJECT_API AInteractable : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractable();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	UFUNCTION(BlueprintCallable)
		void OnInteract();
	UFUNCTION(BlueprintCallable)
		void StopInteract();
	UFUNCTION(BlueprintImplementableEvent)
		void WhenInteracted();
	UFUNCTION(BlueprintImplementableEvent)
		void WhenNotInteracted();

	UFUNCTION(BlueprintCallable)
		void ActivateLoop();
	UFUNCTION(BlueprintCallable)
		void DeActivateLoop();
	/*UFUNCTION(BlueprintCallable)
		void IncreaseCrank();	
	UFUNCTION(BlueprintCallable)
		void DecreaseCrank();
		*/
		 AActivatable* activatedObject;

	 UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Any)
		 TArray<AActivatable*> activatedObjectArray;

	 UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Any)
		 bool isToggle;
	 UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Any)
		 bool needsInteraction;
	 UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Any)
		 bool isTimed;
	 UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Any)
		 bool isCrank;
	 UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Any)
		 bool isCrankRepeatable;
	 UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Any)
		 bool isToggled; 
	 UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Any)
		 bool isOneUse;

	 UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Any)
		 bool activatedBySword;
	 UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Any)
		 bool activatedByBow;

	 UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Any)
		 float crankValue;
	 UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Any)
		 float crankDecreaseValue;
	 bool crankComplete;
};
