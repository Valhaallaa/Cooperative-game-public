// Copyright Epic Games, Inc. All Rights Reserved.

#include "MyProjectCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "Kismet/GameplayStatics.h"
#include "MyProjectGameMode.h"
#include "BowWeapon.h"
#include "SwordWeapon.h"
#include "GameFramework/SpringArmComponent.h"

//////////////////////////////////////////////////////////////////////////
// AMyProjectCharacter

void AMyProjectCharacter::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, FString::Printf(TEXT("BEGIN")));

	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		AProjectile* Proj = Cast<AProjectile>(OtherActor);
		if (Proj) {
			if (Proj->GetShotByPlayer() == false)
				PlayerHealth->TakeDamage(Proj->GetDamage());
		}
		AInteractable* inter = Cast<AInteractable>(OtherActor);
		if (inter) {
			interactedObject = inter;
		}
	}
	

}

void AMyProjectCharacter::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, FString::Printf(TEXT("END")));

	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		AInteractable* inter = Cast<AInteractable>(OtherActor);
		if (inter) {
			interactedObject = nullptr;
		}
	}
}


AMyProjectCharacter::AMyProjectCharacter()
{
	specialAvailable = true;
	
	Hooking = false;
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AMyProjectCharacter::OnOverlapBegin);
	GetCapsuleComponent()->OnComponentEndOverlap.AddDynamic(this, &AMyProjectCharacter::OnOverlapEnd);

	// set our turn rate for input
	TurnRateGamepad = 50.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Create Health Component
	PlayerHealth = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
	PlayerHealth->SetMaxHealth(3);

	// Create Weapon Component
	PlayerWeapon = CreateDefaultSubobject<USwordWeapon>(TEXT("SwordComponent"));
	PlayerWeapon->SetIsPlayer(true);
	PlayerWeapon->SetDamage(1);
	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)


	playerIndication = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	playerIndication->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> FoundMesh(TEXT("/Engine/EditorMeshes/EditorCylinder.EditorCylinder"));
	if (FoundMesh.Succeeded())
	{
		playerIndication->SetStaticMesh(FoundMesh.Object);
	}

	static ConstructorHelpers::FObjectFinder<UMaterial> FoundMaterial(TEXT("Material'/Game/Assets/PlayerRing/PlaterRingMat1.PlaterRingMat1'"));
	if (FoundMaterial.Succeeded())
	{
		primaryPlayerMat = FoundMaterial.Object;
	}

	static ConstructorHelpers::FObjectFinder<UMaterial> FoundMaterial2(TEXT("Material'/Game/Assets/PlayerRing/PlaterRingMat.PlaterRingMat'"));
	if (FoundMaterial.Succeeded())
	{
		secondaryPlayerMat = FoundMaterial2.Object;
	}
	playerIndication->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	//primaryDynamicMaterial = UMaterialInstanceDynamic::Create(primaryPlayerMat, playerIndication);
	//secondaryDynamicMaterial = UMaterialInstanceDynamic::Create(secondaryPlayerMat, playerIndication);

}

//////////////////////////////////////////////////////////////////////////
// Input

void AMyProjectCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("Move Forward / Backward", this, &AMyProjectCharacter::MoveForward);
	PlayerInputComponent->BindAxis("Move Right / Left", this, &AMyProjectCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	/*
	PlayerInputComponent->BindAxis("Turn Right / Left Mouse", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("Turn Right / Left Gamepad", this, &AMyProjectCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("Look Up / Down Mouse", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Look Up / Down Gamepad", this, &AMyProjectCharacter::LookUpAtRate);
	*/
	PlayerInputComponent->BindAxis("Look Right / Left", this, &AMyProjectCharacter::AimLeftRight);
	PlayerInputComponent->BindAxis("Look Up / Down", this, &AMyProjectCharacter::AimUpDown);
	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &AMyProjectCharacter::Attack);
	PlayerInputComponent->BindAction("MouseAttack", IE_Pressed, this, &AMyProjectCharacter::MouseAttack);
	PlayerInputComponent->BindAction("Swap", IE_Pressed, this, &AMyProjectCharacter::Swap);
	PlayerInputComponent->BindAction("Dodge", IE_Pressed, this, &AMyProjectCharacter::Dodge);
	PlayerInputComponent->BindAction("Recall AI", IE_Pressed, this, &AMyProjectCharacter::RecallAI);
	PlayerInputComponent->BindAction("Pause", IE_Pressed, this, &AMyProjectCharacter::Pause);
	PlayerInputComponent->BindAction("Save", IE_Pressed, this, &AMyProjectCharacter::Save);
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &AMyProjectCharacter::Interact);
	PlayerInputComponent->BindAction("ResetProgress", IE_Pressed, this, &AMyProjectCharacter::ResetProgress);
	PlayerInputComponent->BindAction("Special", IE_Pressed, this, &AMyProjectCharacter::SpecialDecider);



	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &AMyProjectCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &AMyProjectCharacter::TouchStopped);
}

void AMyProjectCharacter::SpecialDecider() {
	if (specialAvailable) {
		specialAvailable = false;
		if (isRanged) {
			BowSpecial();
		}
		else {
			MeleeSpecial();
		}
		FTimerHandle activeTimer;
		GetWorldTimerManager().SetTimer(activeTimer, this, &AMyProjectCharacter::SpecialCooldown, 2.0f, false, 2.0f);
	}
}

void AMyProjectCharacter::MeleeSpecial() {

}

void AMyProjectCharacter::SpecialCooldown() {
	specialAvailable = true;
}

void AMyProjectCharacter::BowSpecial() {

	
	TArray<AActor*> overlappingActors;
	UKismetSystemLibrary::SphereOverlapActors(GetWorld(), GetActorLocation(), BowSpecialRadius, TArray<TEnumAsByte<EObjectTypeQuery>>(), AActor::StaticClass(), TArray<AActor*>(), overlappingActors);
	TArray<AActor*> HookableActors;
	for (int i = 0; i < overlappingActors.Num(); i++)
	{
		if (overlappingActors[i] != nullptr) {
			if (overlappingActors[i]->Tags.Contains("HookPoint")) {
				if (FVector::Distance(GetActorLocation(), overlappingActors[i]->GetActorLocation()) >= (BowSpecialRadius / 4))
					HookableActors.Add(overlappingActors[i]);
			}
		}
	}
	AActor* Target = nullptr;
	if (HookableActors.Num() > 0) {
		for (int j = 0; j < HookableActors.Num(); j++)
		{
			if (HookableActors[j] != nullptr) {
				if (j == 0 || Target == nullptr)
					Target = HookableActors[j];
				else {
					if (FVector::Distance(GetActorLocation(), Target->GetActorLocation()) > FVector::Distance(GetActorLocation(), HookableActors[j]->GetActorLocation()))
					{
						Target = HookableActors[j];
					}
				}
			}
		}
	}
	FHitResult outHit;
	if (Target != nullptr) {
		FVector start = GetActorLocation();
		FVector end = Target->GetActorLocation();
		FCollisionQueryParams TraceParams;
		TraceParams.AddIgnoredActor(this);
		TraceParams.AddIgnoredActor(Target);
		TraceParams.bTraceComplex = true;



		DrawDebugLine(GetWorld(), start, end, FColor::Blue, false, 1.0f, 0, 2.0f);
		if (GetWorld()->LineTraceSingleByChannel(outHit, start, end, ECC_Visibility, TraceParams)) {
			PlayerGrappleFailFeedback();
		}
		else {
			HookTarget = Target;
			if (HookTarget != nullptr) {
				Hooking = true;
				PlayerGrapplePassFeedback();
				GetCharacterMovement()->GravityScale = 0.0f;
				GetCharacterMovement()->DisableMovement();

			}
		}
	}
	else
		PlayerGrappleFailFeedback();
}

void AMyProjectCharacter::Pause()
{
	AMyProjectGameMode* gameMode = Cast<AMyProjectGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (gameMode)
		gameMode->PauseGame();
}

void AMyProjectCharacter::Save()
{
	AMyProjectGameMode* gameMode = Cast<AMyProjectGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (gameMode)
		gameMode->SaveGame();
}

void AMyProjectCharacter::ResetProgress()
{
	AMyProjectGameMode* gameMode = Cast<AMyProjectGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (gameMode)
		gameMode->ResetProgress();
}

void AMyProjectCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
	Jump();
}

void AMyProjectCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
	StopJumping();
}

void AMyProjectCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds());
}

void AMyProjectCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds());
}


void AMyProjectCharacter::Attack()
{
	if (PlayerWeapon) {
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, FString::Printf(TEXT("My Direction is: %s"), *aimDirection.ToString()));

		PlayerWeapon->AttemptAttack(aimDirection);
	}
}

void AMyProjectCharacter::MouseAttack() {

	float x;
	float y;
	FHitResult outHit;
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetMousePosition(x, y);
	FVector2D mousePos = FVector2D(x, y);
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHitResultAtScreenPosition(mousePos, TArray<TEnumAsByte<EObjectTypeQuery>>(), true,outHit);

	FVector ActorLoc = GetActorLocation();

	FVector mouseDir = (outHit.ImpactPoint - ActorLoc);
	mouseDir.Normalize();
	mouseDir.Z = 0;
	PlayerWeapon->AttemptAttack(mouseDir);


}

void AMyProjectCharacter::Dodge()
{

}

void AMyProjectCharacter::Interact()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, FString::Printf(TEXT("Interact")));

	if (interactedObject && interactedObject->needsInteraction)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, FString::Printf(TEXT("Interact Object")));

		interactedObject->OnInteract();
	}
}

void AMyProjectCharacter::EndDodge()
{
}

void AMyProjectCharacter::Swap()
{
	AMyProjectGameMode* gameMode = Cast<AMyProjectGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if(gameMode)
	gameMode->SwapPlayerCharacter(startingRotation);
}

void AMyProjectCharacter::RecallAI()
{
	AMyProjectGameMode* gameMode = Cast<AMyProjectGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (gameMode)
		gameMode->RecallCharacter();
}

void AMyProjectCharacter::AimLeftRight(float Value)
{

	aimDirection.X = Value;

}

void AMyProjectCharacter::SetPlayerIndication(bool Value)
{
	if (Value) {
		playerIndication->SetVisibility(true, false);
		GetController();
		if (GetController() != nullptr) { // if player controller
			if (GetController() == UGameplayStatics::GetPlayerController(GetWorld(), 0)) { // controller 0 
				
					primaryDynamicMaterial = UMaterialInstanceDynamic::Create(primaryPlayerMat, playerIndication);
				if(playerIndication != nullptr)
					playerIndication->SetMaterial(0, primaryDynamicMaterial);
			}
			else if(GetController() == UGameplayStatics::GetPlayerController(GetWorld(), 1)){ // controller 1
				
					secondaryDynamicMaterial = UMaterialInstanceDynamic::Create(secondaryPlayerMat, playerIndication);
				if (playerIndication != nullptr)
					playerIndication->SetMaterial(0, secondaryDynamicMaterial);
			}
			else {
				// something went wrong
			}
		}
	}
	else {
		playerIndication->SetVisibility(false, false);
	}
}

void AMyProjectCharacter::BeginPlay()
{
	Super::BeginPlay();

	startingRotation = GetActorRotation();

	AMyProjectGameMode* gameMode = Cast<AMyProjectGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	

	if (isRanged) {

		PlayerWeapon->DestroyComponent();
		PlayerWeapon = NewObject<UBowWeapon>(this,UBowWeapon::StaticClass(), TEXT("BowWeapon"));
		PlayerWeapon->RegisterComponent();
		PlayerWeapon->SetIsPlayer(true);
		PlayerWeapon->SetDamage(1);
		if (gameMode)
			gameMode->AddCharacter(this,0);
	}else{
		if (gameMode)
			gameMode->AddCharacter(this,1);
		//AddComponentByClass(USwordWeapon::StaticClass(), false, this->GetTransform(), false);
		//PlayerWeapon = CreateDefaultSubobject<USwordWeapon>(TEXT("SwordWeapon"));
	} 
}



void AMyProjectCharacter::AimUpDown(float Value)
{

	aimDirection.Y = Value;


}

void AMyProjectCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AMyProjectCharacter::MoveRight(float Value)
{
	if ( (Controller != nullptr) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}
void AMyProjectCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Hooking) {
		SetActorLocation(FMath::Lerp(GetActorLocation(), HookTarget->GetActorLocation(),DeltaTime));
		if (FVector::Distance(GetActorLocation(), HookTarget->GetActorLocation()) < 100.0f) {
			Hooking = false;
			GetCharacterMovement()->GravityScale = 1.75f;
			GetCharacterMovement()->SetMovementMode(MOVE_Walking);
			


		}
	}
	FVector indicationLocation = FVector(0, 0, 0);
	FHitResult outHit;
	FVector start = GetActorLocation();
	FVector end = GetActorLocation() - FVector(0, 0, 500);
	FCollisionQueryParams TraceParams;
	TraceParams.AddIgnoredActor(this);
	
	TraceParams.bTraceComplex = true;
	if (GetWorld()->LineTraceSingleByChannel(outHit, start, end, ECC_Visibility, TraceParams)) {
		// Obstruction
		indicationLocation = outHit.ImpactPoint;
		indicationLocation += FVector(0, 0, 10);
		playerIndication->SetWorldLocation(indicationLocation);
	}
	FRotator rotation;
	rotation = UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetControlRotation();
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, FString::Printf(TEXT("My ControlDirection is: %s"), *rotation.ToString()));
	
}