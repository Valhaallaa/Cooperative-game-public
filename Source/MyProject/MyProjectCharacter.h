// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "HealthComponent.h"
#include "WeaponComponent.h"
#include "Interactable.h"
#include "MyProjectCharacter.generated.h"

UCLASS(config=Game)
class AMyProjectCharacter : public ACharacter
{
	GENERATED_BODY()

		bool specialAvailable;

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere,Category ="Player Indicator")
	class UStaticMeshComponent* playerIndication;

	UMaterial* primaryPlayerMat;

	UMaterial* secondaryPlayerMat;

	AActor* HookTarget;
	bool Hooking;
	

	UMaterialInstanceDynamic* primaryDynamicMaterial;
	UMaterialInstanceDynamic* secondaryDynamicMaterial;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Character, meta = (AllowPrivateAccess = "true"))
		class UHealthComponent* PlayerHealth;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Character, meta = (AllowPrivateAccess = "true"))
		class UWeaponComponent* PlayerWeapon;

	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	AMyProjectCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Input)
	float TurnRateGamepad;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character, meta = (AllowPrivateAccess = "true"))
		float BowSpecialRadius;

	void SetPlayerIndication(bool Value);

protected:

	virtual void BeginPlay() override;

	FRotator GetStartingRotation();

	void AimUpDown(float Value);

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character, meta = (AllowPrivateAccess = "true"))
		FVector aimDirection;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character, meta = (AllowPrivateAccess = "true"))
		bool isRanged;

	FRotator startingRotation;

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);



	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	void Attack();

	void MouseAttack();

	void Dodge();
	
	void Interact();

	void EndDodge();

	void Swap();

	void RecallAI();

	void AimLeftRight(float Value);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

	

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void SpecialDecider();
	void MeleeSpecial();
	void SpecialCooldown();
	void BowSpecial();
	// End of APawn interface

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	UFUNCTION(BlueprintImplementableEvent)
		void PlayerDamageFeedback();
	UFUNCTION(BlueprintImplementableEvent)
		void PlayerGrappleFailFeedback();
	UFUNCTION(BlueprintImplementableEvent)
		void PlayerGrapplePassFeedback();
		UFUNCTION(BlueprintImplementableEvent)
		void PlayerRecallAI();

		void Pause();
		void Save();
		void ResetProgress();

		bool inInteract;

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
			AInteractable* interactedObject;
};

