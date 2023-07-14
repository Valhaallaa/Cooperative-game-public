// Fill out your copyright notice in the Description page of Project Settings.


#include "SwordWeapon.h"
#include "DrawDebugHelpers.h"
#include "AICharacter.h"
#include "Interactable.h"
#include "Kismet/GameplayStatics.h"
#include "MyProjectCharacter.h"

void USwordWeapon::Attack(FVector direction)
{


	TArray<AActor*> overlappingActors;
	if (direction == FVector::ZeroVector)
		direction =GetOwner()->GetActorForwardVector();

	direction = direction * 100;

		

	FVector SpherePosition = (GetOwner()->GetActorLocation() + direction);
	float Radius = 50.0f;
	DrawDebugSphere(GetWorld(), SpherePosition, Radius,32,FColor::Red,true,5.0f,0);
	UKismetSystemLibrary::SphereOverlapActors(GetWorld(), SpherePosition, Radius, TArray<TEnumAsByte<EObjectTypeQuery>>(), AActor::StaticClass(), TArray<AActor*>(), overlappingActors);
	for (int i = 0; i < overlappingActors.Num(); i++)
	{
		if (overlappingActors[i] != nullptr) {
			if (overlappingActors[i]->Tags.Contains("Cuttable")) {
				FHitResult outHit;
				FVector start = GetOwner()->GetActorLocation();
				FVector end = overlappingActors[i]->GetActorLocation();
				FCollisionQueryParams TraceParams;
				TraceParams.AddIgnoredActor(GetOwner());
				TraceParams.AddIgnoredActor(overlappingActors[i]);
				TraceParams.bTraceComplex = true;
				DrawDebugLine(GetWorld(), start, end, FColor::Blue, false, 1.0f, 0, 2.0f);
				if (GetWorld()->LineTraceSingleByChannel(outHit, start, end, ECC_Visibility, TraceParams)) {
					// Obstruction
				}
				else {
					// It hit the target.
					GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Hit Cuttable")));
					AInteractable* Int = Cast<AInteractable>(overlappingActors[i]);
					if (Int) {
						GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("INT"), *outHit.GetActor()->GetName()));
						if (Int->activatedBySword) {
							Int->OnInteract();
						}
					}
				}
			}
		}
	}
	//UKismetSystemLibrary::SphereOverlapActors(GetWorld(), GetOwner()->GetActorLocation(), BowSpecialRadius, TArray<TEnumAsByte<EObjectTypeQuery>>(), actorClass, TArray<AActor*>(), overlappingActors);
	/*
	if (direction == FVector::ZeroVector) {
		direction = GetOwner()->GetActorForwardVector();
	}

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Sword Attack"));
	FHitResult outHit;
	FVector start = GetOwner()->GetActorLocation();
	FVector end = start + direction * attackRange;
	FCollisionQueryParams TraceParams;
	TraceParams.AddIgnoredActor(GetOwner());
	TraceParams.bTraceComplex = true;



	DrawDebugLine(GetWorld(), start, end, FColor::Blue, false, 1.0f, 0, 2.0f);
	if (GetWorld()->LineTraceSingleByChannel(outHit, start, end, ECC_Visibility, TraceParams))
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("You are hitting: %s"), *outHit.GetActor()->GetName()));
		TArray<UHealthComponent> HealthArray;
		UHealthComponent* HealthCheck;
		HealthCheck = outHit.GetActor()->FindComponentByClass<UHealthComponent>();
		AActor* HitActor = outHit.GetActor();

		if (HealthCheck)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Has Health Component")));

			AMyProjectCharacter* Char = Cast<AMyProjectCharacter>(HitActor);
			if (Char) {
				if (!byPlayer) {
					// AI hit Player
					
						HealthCheck->TakeDamage(weaponDamage);
				}
				else {
					// Player hit Player

				}
				

			}
			else {
				//Player Hit other with healthComponent
				if(byPlayer)
					HealthCheck->TakeDamage(weaponDamage);


			}
		}
		
		AInteractable* Int = Cast<AInteractable>(HitActor);
		if (Int) {
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("INT"), *outHit.GetActor()->GetName()));

			if (Int->activatedBySword) {
				Int->OnInteract();
			}

		}

		/*
		AProjectile* Proj = Cast<AProjectile>(outHit);
		if (Proj != nullptr) 
		{

		}
		
	}
	*/

}

USwordWeapon::USwordWeapon()
{
	weaponDamage = 300;
	attackCooldown = 1.5f;
	attackRange = 250.0f;
	onCooldown = false;
}
