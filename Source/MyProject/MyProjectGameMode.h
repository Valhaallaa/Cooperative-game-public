// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Collectable.h"
#include "SharedCameraActor.h"
#include "MyProjectGameMode.generated.h"

UCLASS(minimalapi)
class AMyProjectGameMode : public AGameModeBase
{
	GENERATED_BODY()

	
	int possessedCharacter;
	AController* AIController;
	ASharedCameraActor* sharedCamera;
	FRotator controlRotation;
	bool rotationModified;

	
	FVector P0RespawnPosition;
	FVector P1RespawnPosition;

	
	

public:
	AMyProjectGameMode();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool twoPlayer;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool controllerOffset;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool initialAddComplete;

	//TArray<APawn*> playerCharacters;
	TStaticArray<APawn*, 2> playerCharacters;
	void AddCharacter(APawn* Value,int iValue);
	UFUNCTION(BlueprintCallable)
	void AddPlayer();
	UFUNCTION(BlueprintCallable)
	void RemovePlayer();
	UFUNCTION(BlueprintCallable)
	void ChangePlayerOrientation(FRotator RotatorValue, float zoomAmount);
	UFUNCTION(BlueprintCallable)
	void SwapControlMethod();

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void UpdateP0Respawn(FVector Position);
	UFUNCTION(BlueprintCallable)
	bool GetTwoPlayer();
	UFUNCTION(BlueprintCallable)
	void UpdateP1Respawn(FVector Position);
	UFUNCTION(BlueprintCallable)
	FVector GetP0Respawn();
	UFUNCTION(BlueprintCallable)
	FVector GetP1Respawn();
	UFUNCTION(BlueprintCallable)
	void UpdateBothRespawn(FVector Player0Position, FVector Player1Position);

	UFUNCTION(BlueprintCallable)
	APawn* GetCharacter(int iValue);
	void AddSharedCamera(ASharedCameraActor* Value);
	void RecallCharacter();
	void ClearCharacter();
	void SwapPlayerCharacter(FRotator Value);

	void AIPossessPlayer0();

	void AIPossessPlayer1();

	FVector GetCharactersAverage();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Any)
		TArray<TSubclassOf<class ACollectable>> collectablesFound;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool L1_CollComplete;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool L2_CollComplete;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool L3_CollComplete;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		int L1_CollectedNum;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		int L2_CollectedNum;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		int L3_CollectedNum;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		int L1_TotalCollNum;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		int L2_TotalCollNum;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		int L3_TotalCollNum;


	UFUNCTION(BlueprintCallable)
		void AddCollectable(int levelID);

	UFUNCTION(BlueprintCallable)
		void SetCollTotalNum(int levelID, int totalNum);
	int GetLevelID();

	UFUNCTION(BlueprintImplementableEvent)
		void SaveAddPlayer();
	UFUNCTION(BlueprintImplementableEvent)
		void PauseGame();
	UFUNCTION(BlueprintImplementableEvent)
		void SaveGame();	
	UFUNCTION(BlueprintImplementableEvent)
		void LoadGame();
	UFUNCTION(BlueprintImplementableEvent)
		void ResetProgress();
	UFUNCTION(BlueprintImplementableEvent)
		void MovePlayer();
};



