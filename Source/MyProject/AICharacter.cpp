// Fill out your copyright notice in the Description page of Project Settings.


#include "AICharacter.h"
#include "BowWeapon.h"
#include "SwordWeapon.h"
#include "Components/CapsuleComponent.h"
#include "Projectile.h"

void AAICharacter::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("AI OverlapEventbegin"));
		AProjectile* Proj = Cast<AProjectile>(OtherActor);
		if (Proj && Proj->GetShotByPlayer()) {
			AIHealth->TakeDamage(Proj->GetDamage());
		}
	}
}


// Sets default values
AAICharacter::AAICharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AAICharacter::OnOverlapBegin);


	// Create Health Component
	AIHealth = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
	AIHealth->SetMaxHealth(2);

	// Create Weapon Component
	AIWeapon = CreateDefaultSubobject<USwordWeapon>(TEXT("SwordComponent"));
	AIWeapon->SetIsPlayer(false);
	AIWeapon->SetDamage(1);
}

// Called when the game starts or when spawned
void AAICharacter::BeginPlay()
{
	Super::BeginPlay();
	

	if (isRanged) {

		AIWeapon->DestroyComponent();
		AIWeapon = NewObject<UBowWeapon>(this, UBowWeapon::StaticClass(), TEXT("BowWeapon"));
		AIWeapon->RegisterComponent();
		AIWeapon->SetIsPlayer(false);
		AIWeapon->SetDamage(1);
	}
	else {
		
	}

}

// Called every frame
void AAICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AAICharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

