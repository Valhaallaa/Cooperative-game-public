// Fill out your copyright notice in the Description page of Project Settings.

#include "SharedCameraActor.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "MyProjectGameMode.h"


// Sets default values
ASharedCameraActor::ASharedCameraActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Sphere = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Sphere"));
	ConstructorHelpers::FObjectFinder<UStaticMesh>SphereMeshAsset(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
	Sphere->SetStaticMesh(SphereMeshAsset.Object);
	Sphere->SetWorldScale3D(FVector(0.3f));
	Sphere->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	RootComponent = Sphere;


	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

}

UCameraComponent* ASharedCameraActor::GetCamera()
{
	return FollowCamera;
}

// Called when the game starts or when spawned
void ASharedCameraActor::BeginPlay()
{
	Super::BeginPlay();
	AMyProjectGameMode* gameMode = Cast<AMyProjectGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (gameMode)
		gameMode->AddSharedCamera(this);
}

// Called every frame
void ASharedCameraActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AMyProjectGameMode* gameMode = Cast<AMyProjectGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	if (gameMode) {
		this->SetActorLocation(gameMode->GetCharactersAverage());
		float distance = FVector::Distance(gameMode->GetCharacter(0)->GetActorLocation(), gameMode->GetCharacter(1)->GetActorLocation());
		distance = FMath::Clamp(distance, 1000.0f, 20000.0f);
		CameraBoom->TargetArmLength = distance * zoomOutAmount;

	}
}

