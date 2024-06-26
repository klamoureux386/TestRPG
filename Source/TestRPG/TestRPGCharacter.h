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
	UPROPERTY(Category = "Utility", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USurroundingsChecker* SurroundingsChecker;

	/** SurroundingsChecker sub-objects */
	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Utility, meta = (AllowPrivateAccess = "true"))
	//	class USceneComponent* ActorCenter;

	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Utility, meta = (AllowPrivateAccess = "true"))
	//	class USceneComponent* FrontRaycastPos;

	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Utility, meta = (AllowPrivateAccess = "true"))
	//	class USceneComponent* BackRaycastPos;


public:
	ATestRPGCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Input")
	float TurnRateGamepad;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
	bool isSliding = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
	FVector slideDirection = FVector(0, 0, 0);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Debug")
	bool ShowDebug = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
	float baseMaxWalkSpeed = 700.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
	float baseSlideSpeed = 1000.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
	float maxSlideSpeed = 1300.f;

	//https://www.youtube.com/watch?v=oe2vPXvFLpI
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats", meta = (DisplayName = "Jump Curve"))
	UCurveFloat* m_jump_curve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats", meta = (DisplayName = "Slide Acceleration Curve"))
	UCurveFloat* slideAccelerationCurve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats", meta = (DisplayName = "Slide Speed Curve"))
	UCurveFloat* slideSpeedCurve;

	float forwardInput = 0.0f;
	float rightInput = 0.0f;

protected:

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** Start Slide */
	void StartSlide();

	void AdjustSlide();

	virtual void Jump() override;

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

