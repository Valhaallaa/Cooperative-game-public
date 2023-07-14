// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Projectile.generated.h"

UCLASS()
class MYPROJECT_API AProjectile : public AActor
{
	GENERATED_BODY()

		UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* Sphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Character, meta = (AllowPrivateAccess = "true"))
		class USphereComponent* Hitbox;

	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:	
	// Sets default values for this actor's properties
	AProjectile();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Statistics, meta = (AllowPrivateAccess = "true"))
		int projectileDamage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Statistics, meta = (AllowPrivateAccess = "true"))
		bool shotByPlayer;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Statistics, meta = (AllowPrivateAccess = "true"))
		float speed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Statistics, meta = (AllowPrivateAccess = "true"))
		FVector direction;

	UMaterial* StoredMat;
	UMaterialInstanceDynamic* DynamicMat;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetupCollisions();
	void SetDamage(int Value);
	void SetSpeed(float Value);
	void SetDirection(FVector Direction);
	void SetIfShotByPlayer(bool Player);
	int GetDamage();
	bool GetShotByPlayer();
};
