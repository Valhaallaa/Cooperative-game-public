// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Activatable.generated.h"

UCLASS()
class MYPROJECT_API AActivatable : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AActivatable();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void OnActivate();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void StopActivate();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadWrite, Category = Any)
		bool isActivated;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Any)
		int interactsNeeded;

	UPROPERTY(BlueprintReadWrite, Category = Any)
		int interactsActive;
};
