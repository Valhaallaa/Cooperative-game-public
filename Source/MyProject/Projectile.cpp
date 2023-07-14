// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "Interactable.h"

void AProjectile::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Bullet OverlapEventbegin"));

		AProjectile* Proj = Cast<AProjectile>(OtherActor);
		if (Proj != nullptr) {

		}
		else if (OtherActor->ActorHasTag("Player") && shotByPlayer) {

		}
		else if (OtherActor->ActorHasTag("AI") && !shotByPlayer) {

		}
		else if (OtherActor->ActorHasTag("Trigger"))
		{
		}
		else
		{
			AInteractable* Int = Cast<AInteractable>(OtherActor);
			if (Int) {

				if (Int->activatedByBow) {
					Int->OnInteract();
				}

			}
			Destroy();
		}
	}
}

void AProjectile::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Sphere = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Sphere"));
	//Sphere->AttachToComponent(GetRootComponent());
	ConstructorHelpers::FObjectFinder<UStaticMesh>SphereMeshAsset(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
	Sphere->SetStaticMesh(SphereMeshAsset.Object);
	Sphere->SetWorldScale3D(FVector(0.3f));
	Sphere->SetCollisionProfileName(TEXT("OverlapAllDynamic"));

	//UMaterialInterface bulletMaterial;
	/*
	static ConstructorHelpers::FObjectFinder<UMaterial> bulletMaterial(TEXT("Material'/Game/BulletMaterial.BulletMaterial'"));

	if (bulletMaterial.Succeeded())
		StoredMat = bulletMaterial.Object;
	DynamicMat = UMaterialInstanceDynamic::Create(StoredMat, Sphere);
	*/
	//UMaterialInterface* mat = bulletMaterial.Object;

	Sphere->SetMaterial(0, DynamicMat);
	RootComponent = Sphere;

	Hitbox = CreateDefaultSubobject<USphereComponent>(TEXT("Hitbox"));
	Hitbox->InitSphereRadius(5.f);
	Hitbox->SetCollisionProfileName(TEXT("OverlapAll"));
	Hitbox->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FVector location = GetActorLocation();
	FVector NextLocation = location + (direction * speed);
	SetActorLocation(NextLocation);
}

void AProjectile::SetupCollisions()
{
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnOverlapBegin);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &AProjectile::OnOverlapEnd);
}

void AProjectile::SetDamage(int Value)
{
	projectileDamage = Value;
}

void AProjectile::SetSpeed(float Value)
{
	speed = Value;
}

void AProjectile::SetDirection(FVector Direction)
{
	direction = FVector(Direction.X, Direction.Y, 0);

}

void AProjectile::SetIfShotByPlayer(bool Player)
{
	shotByPlayer = Player;
}

int AProjectile::GetDamage()
{
	return projectileDamage;
}

bool AProjectile::GetShotByPlayer()
{
	return shotByPlayer;
}

