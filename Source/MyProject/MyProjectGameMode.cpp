// Copyright Epic Games, Inc. All Rights Reserved.

#include "MyProjectGameMode.h"
#include "MyProjectCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "GameMapsSettings.h"
#include "Kismet/GameplayStatics.h"


AMyProjectGameMode::AMyProjectGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
	twoPlayer = false;
	controllerOffset = false;
	rotationModified = false;
	initialAddComplete = false;
}




void AMyProjectGameMode::AddCharacter(APawn* Value, int iValue)
{
	GEngine->ForceGarbageCollection(true);
	
	playerCharacters[iValue] = Value;
	if (!initialAddComplete)
		controlRotation = FRotator(0, -90, 0);
		//UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetControlRotation();
	if(Value->GetController() != nullptr)
		Value->GetController()->UnPossess();

	if (!initialAddComplete) {
		if (UGameplayStatics::GetPlayerController(GetWorld(), iValue) == nullptr) { // check if player character index has a match controller index
			//Value->GetController()->UnPossess();
			if (AIController == nullptr) { // checks if ai player is a thing if not creates it and posses the character with no controller
				Value->SpawnDefaultController();
				AIController = playerCharacters[iValue]->GetController();
				AMyProjectCharacter* Char = Cast<AMyProjectCharacter>(playerCharacters[iValue]);
				Char->SetPlayerIndication(false);
			}
			else {
				AIController->UnPossess();
				AIController->Possess(playerCharacters[iValue]);
				AMyProjectCharacter* Char = Cast<AMyProjectCharacter>(playerCharacters[iValue]);
				Char->SetPlayerIndication(false);
			}

		}
		else {
			UGameplayStatics::GetPlayerController(GetWorld(), iValue)->Possess(playerCharacters[iValue]); // posses player with matching controller
			UGameplayStatics::GetPlayerController(GetWorld(), iValue)->SetControlRotation(controlRotation);
			possessedCharacter = iValue;
			if (playerCharacters[iValue] != nullptr) {
				AMyProjectCharacter* Char = Cast<AMyProjectCharacter>(playerCharacters[iValue]);
				Char->SetPlayerIndication(true);
				
			}
			if (sharedCamera != nullptr)
				UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetViewTarget(sharedCamera);
		}
	}
	else {
		if (possessedCharacter == iValue) {
			UGameplayStatics::GetPlayerController(GetWorld(), 0)->Possess(playerCharacters[iValue]); // posses player 0 with their respawning character
			UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetControlRotation(controlRotation);
			if (playerCharacters[iValue] != nullptr) {
				AMyProjectCharacter* Char = Cast<AMyProjectCharacter>(playerCharacters[iValue]);
				Char->SetPlayerIndication(true);
			}
		}
		else if (UGameplayStatics::GetPlayerController(GetWorld(), 1) != nullptr && possessedCharacter != iValue) { // if player 1 exists and the respawning character is not player 0
			UGameplayStatics::GetPlayerController(GetWorld(), 1)->Possess(playerCharacters[iValue]); // posses player 0 with their respawning character
			UGameplayStatics::GetPlayerController(GetWorld(), 1)->SetControlRotation(controlRotation);
			if (playerCharacters[iValue] != nullptr) {
				AMyProjectCharacter* Char = Cast<AMyProjectCharacter>(playerCharacters[iValue]);
				Char->SetPlayerIndication(true);
			}
		}
		else if(UGameplayStatics::GetPlayerController(GetWorld(), 1) == nullptr && possessedCharacter != iValue){ // no player 1 and its not player 0 character respawing
			AIController->Possess(playerCharacters[iValue]);
			if (playerCharacters[iValue] != nullptr) {
				AMyProjectCharacter* Char = Cast<AMyProjectCharacter>(playerCharacters[iValue]);
				Char->SetPlayerIndication(false);
			}
		}
		if (sharedCamera != nullptr)
			UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetViewTarget(sharedCamera);

	}
	if (!initialAddComplete) {
		if (iValue == 1)
			P1RespawnPosition = Value->GetActorLocation();
		else if (iValue == 0)
			P0RespawnPosition = Value->GetActorLocation();
		if (playerCharacters[0] != nullptr && playerCharacters[1] != nullptr)
			initialAddComplete = true;
	}

	if (initialAddComplete) {
		if (AIController == nullptr) {
			if (playerCharacters[0]->GetController() == nullptr) {
				playerCharacters[0]->SpawnDefaultController();
				AIController = playerCharacters[0]->GetController();
				AMyProjectCharacter* Char = Cast<AMyProjectCharacter>(playerCharacters[0]);
				Char->SetPlayerIndication(false);
			}
			else if (playerCharacters[1]->GetController() == nullptr) {
				playerCharacters[1]->SpawnDefaultController();
				AIController = playerCharacters[1]->GetController();
				AMyProjectCharacter* Char = Cast<AMyProjectCharacter>(playerCharacters[1]);
				Char->SetPlayerIndication(false);
			}
		}
	}
}

void AMyProjectGameMode::AddPlayer()
{
	if (UGameplayStatics::GetPlayerController(GetWorld(), 1) == nullptr) { // creates controller if it isnt there
		UGameplayStatics::CreatePlayer(GetWorld(),1);
		twoPlayer = true;

	}
	else {
		twoPlayer = true;
	}
	// assigns controller to non possed character
	if (possessedCharacter == 1) {
		if (playerCharacters[0] != nullptr) {
			AIController->UnPossess();
			UGameplayStatics::GetPlayerController(GetWorld(), 1)->Possess(playerCharacters[0]);
			AMyProjectCharacter* Char = Cast<AMyProjectCharacter>(playerCharacters[0]);
			Char->SetPlayerIndication(true);
		}
	}
	else {
		if (playerCharacters[1] != nullptr) {
			AIController->UnPossess();
			UGameplayStatics::GetPlayerController(GetWorld(), 1)->Possess(playerCharacters[1]);
			AMyProjectCharacter* Char = Cast<AMyProjectCharacter>(playerCharacters[1]);
			Char->SetPlayerIndication(true);
		}
	}
	SaveAddPlayer();
	UGameplayStatics::GetPlayerController(GetWorld(), 1)->SetControlRotation(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetControlRotation());
}

void AMyProjectGameMode::RemovePlayer()
{ // removes player 1
	if (UGameplayStatics::GetPlayerController(GetWorld(), 1) != nullptr) {
		UGameplayStatics::GetPlayerController(GetWorld(), 1)->UnPossess();
		UGameplayStatics::RemovePlayer(UGameplayStatics::GetPlayerController(GetWorld(), 1),true);
		twoPlayer = false;
		SaveAddPlayer();
	}

	// Assign AI to character that isnt possessed

	if (possessedCharacter == 1) {
		if (playerCharacters[0] != nullptr) {
			AMyProjectCharacter* Char = Cast<AMyProjectCharacter>(playerCharacters[0]);
			Char->SetPlayerIndication(false);
			AIPossessPlayer0();
		}
	}
	else {
		if (playerCharacters[1] != nullptr) {
			AMyProjectCharacter* Char = Cast<AMyProjectCharacter>(playerCharacters[1]);
			Char->SetPlayerIndication(false);
			AIPossessPlayer1();
		}
	}
}

void AMyProjectGameMode::ChangePlayerOrientation(FRotator RotatorValue, float zoomAmount) {

	controlRotation = RotatorValue;

	UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetControlRotation(controlRotation);

	FRotator Offset = FRotator(0, 0, -90);
	rotationModified = true;

	if (UGameplayStatics::GetPlayerController(GetWorld(), 1) != nullptr)
		UGameplayStatics::GetPlayerController(GetWorld(), 1)->SetControlRotation(controlRotation);
	if (sharedCamera) {
		sharedCamera->SetActorRotation(controlRotation + FRotator(sharedCamera->GetActorRotation().Pitch, 90, sharedCamera->GetActorRotation().Roll));
		sharedCamera->zoomOutAmount = zoomAmount;
	}
}

void AMyProjectGameMode::SwapControlMethod() {
	/*
	auto Settings = const_cast<UGameMapsSettings*>(GetDefault<UGameMapsSettings>());
	if (Settings) {
		if (controllerOffset) {
			Settings->bOffsetPlayerGamepadIds = true;
			controllerOffset = true;
		}
		else {
			Settings->bOffsetPlayerGamepadIds = false;
			controllerOffset = false;
		}
	}
	*/
}

void AMyProjectGameMode::UpdateP0Respawn(FVector Position)
{
	P0RespawnPosition = Position;
}

void AMyProjectGameMode::UpdateP1Respawn(FVector Position)
{
	P1RespawnPosition = Position;
}

FVector AMyProjectGameMode::GetP0Respawn()
{
	return P0RespawnPosition;
}

FVector AMyProjectGameMode::GetP1Respawn()
{
	return P1RespawnPosition;
}

void AMyProjectGameMode::UpdateBothRespawn(FVector Player0Position, FVector Player1Position)
{
	P0RespawnPosition = Player0Position;
	P1RespawnPosition = Player1Position;
}


APawn* AMyProjectGameMode::GetCharacter(int iValue)
{
	return playerCharacters[iValue];
}

void AMyProjectGameMode::AddSharedCamera(ASharedCameraActor* Value)
{
	sharedCamera = Value;

}

void AMyProjectGameMode::RecallCharacter()
{
	if (possessedCharacter == 1) {
		AMyProjectCharacter* Char = Cast<AMyProjectCharacter>(playerCharacters[0]);
		Char->PlayerRecallAI();
		
	}
	else {
		AMyProjectCharacter* Char = Cast<AMyProjectCharacter>(playerCharacters[1]);
		Char->PlayerRecallAI();
	}
}



void AMyProjectGameMode::ClearCharacter()
{
	playerCharacters[0] = nullptr;
	playerCharacters[1] = nullptr;
}

void AMyProjectGameMode::SwapPlayerCharacter(FRotator Value)
{
	if(!twoPlayer){
	APlayerController* PController;
	AController* CheckCont;
	PController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	controlRotation = UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetControlRotation();
	//if (UGameplayStatics::GetPlayerController(GetWorld(), 1) == nullptr) {

		switch (possessedCharacter)
		{
		case 0:
			CheckCont = playerCharacters[1]->GetController();
			if (playerCharacters[1] != nullptr) {
				PController->UnPossess();
				PController->Possess(playerCharacters[1]);
				PController->SetControlRotation(controlRotation);
				AMyProjectCharacter* Char = Cast<AMyProjectCharacter>(playerCharacters[1]);
				Char->SetPlayerIndication(true);
				//PController->SetControlRotation(Value);
				if (playerCharacters[0] != nullptr)
					playerCharacters[0]->Controller = nullptr;
				FTimerHandle activeTimer;
				if (playerCharacters[0] != nullptr) {
					AMyProjectCharacter* Char2 = Cast<AMyProjectCharacter>(playerCharacters[0]);
					Char2->SetPlayerIndication(false);
				}
				GetWorldTimerManager().SetTimer(activeTimer, this, &AMyProjectGameMode::AIPossessPlayer0, 1.0f, false, 2.0f);

				possessedCharacter = 1;
			}

			break;
		case 1:
			CheckCont = playerCharacters[0]->GetController();
			if (playerCharacters[0] != nullptr) {
				PController->UnPossess();
				PController->Possess(playerCharacters[0]);
				PController->SetControlRotation(controlRotation);
				AMyProjectCharacter* Char = Cast<AMyProjectCharacter>(playerCharacters[0]);
				Char->SetPlayerIndication(true);
				//PController->SetControlRotation(Value);
				if (playerCharacters[1] != nullptr)
					playerCharacters[1]->Controller = nullptr;
				FTimerHandle activeTimer2;
				if (playerCharacters[1] != nullptr) {
					AMyProjectCharacter* Char2 = Cast<AMyProjectCharacter>(playerCharacters[1]);
					Char2->SetPlayerIndication(false);
				}
				GetWorldTimerManager().SetTimer(activeTimer2, this, &AMyProjectGameMode::AIPossessPlayer1, 1.0f, false, 2.0f);

				possessedCharacter = 0;
			}
			break;

		default:
			break;
		}
		if (sharedCamera != nullptr)
			UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetViewTarget(sharedCamera);
	}
}

void AMyProjectGameMode::AIPossessPlayer0() {
	if (playerCharacters[0]->GetController() == nullptr) {
		if (AIController == nullptr) {
			if (playerCharacters[0] != nullptr) {
				playerCharacters[0]->SpawnDefaultController();
				AIController = playerCharacters[0]->GetController();
			}
		}
		else {
			AIController->UnPossess();
			if (playerCharacters[0] != nullptr)
				AIController->Possess(playerCharacters[0]);
		}
	}

}

void AMyProjectGameMode::AIPossessPlayer1() {
	if (playerCharacters[1]->GetController() == nullptr) {
		if (AIController == nullptr) {
			if (playerCharacters[1] != nullptr) {
				playerCharacters[1]->SpawnDefaultController();
				AIController = playerCharacters[1]->GetController();
			}
		}
		else {
			AIController->UnPossess();
			if (playerCharacters[1] != nullptr)
				AIController->Possess(playerCharacters[1]);
		}
	}
}
FVector AMyProjectGameMode::GetCharactersAverage()
{

	TArray<AActor*> ActorCharacters;
	if(playerCharacters[0] != nullptr)
		ActorCharacters.Add(playerCharacters[0]);
	if (playerCharacters[1] != nullptr)
		ActorCharacters.Add(playerCharacters[1]);
	return UGameplayStatics::GetActorArrayAverageLocation(ActorCharacters);
}

bool AMyProjectGameMode::GetTwoPlayer() {
	return twoPlayer;
}

void AMyProjectGameMode::BeginPlay()
{
	Super::BeginPlay();



}

void AMyProjectGameMode::AddCollectable(int levelID)
{

	switch (levelID) {

	case 1:
		L1_CollectedNum++;

		if (L1_CollectedNum == L1_TotalCollNum) {
			L1_CollComplete = true;
		}

		break;
	case 2: 
		L2_CollectedNum++;

		if (L2_CollectedNum == L2_TotalCollNum) {
			L2_CollComplete = true;
		}
		break;
	case 3:
		L3_CollectedNum++;

		if (L3_CollectedNum == L3_TotalCollNum) {
			L3_CollComplete = true;
		}
		break;

	}

}

void AMyProjectGameMode::SetCollTotalNum(int levelID, int totalNum)
{
	switch (levelID) {

	case 1:
		L1_TotalCollNum = totalNum;

		break;
	case 2:
		L2_TotalCollNum = totalNum;

		break;
	case 3:
		L3_TotalCollNum = totalNum;
	}
}

int AMyProjectGameMode::GetLevelID()
{
	return 0;

}
