// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TestRPGCharacter.generated.h"

UCLASS(config = Game)
class ATestRPGCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	/** SurroundingsChecker */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Utility, meta = (AllowPrivateAccess = "true"))
	class USurroundingsChecker* SurroundingsChecker;

public:
	ATestRPGCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Input)
		float TurnRateGamepad;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = State)
		bool isSliding = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = State)
		FVector slideDirection = FVector(0, 0, 0);

	float forwardInput = 0.0f;
	float rightInput = 0.0f;

protected:
	/*class ASurroundingsChecker* surroundingsChecker;*/

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** Start Slide */
	void StartSlide();

	/** End Slide */
	void EndSlide();

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

private:

	FString VelocityAsString();
	FVector GetGroundNormal();
	void DrawDebugLines();
	TArray<AActor*> raycastMaskIgnoreActors;

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

	virtual void Tick(float deltaSeconds) override;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	FORCEINLINE class USurroundingsChecker* GetSurroundingsChecker() const { return SurroundingsChecker; }
};

