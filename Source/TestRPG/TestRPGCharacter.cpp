// Copyright Epic Games, Inc. All Rights Reserved.

#include "TestRPGCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"

#include "SurroundingsChecker.h"

//////////////////////////////////////////////////////////////////////////
// ATestRPGCharacter

ATestRPGCharacter::ATestRPGCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

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
	GetCharacterMovement()->MaxWalkSpeed = baseMaxWalkSpeed;
	GetCharacterMovement()->MaxAcceleration = 1500.0f;
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

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)

	/*FrontRaycastPos = CreateDefaultSubobject<UActorComponent>(TEXT("FrontRaycastPos"));*/


	//Set up Utility components
	raycastMaskIgnoreActors = TArray<AActor*>();
	raycastMaskIgnoreActors.Push(this);

	//SurroundingsChecker Setup
	SurroundingsChecker = CreateDefaultSubobject<USurroundingsChecker>(TEXT("SurroundingsChecker"));
	SurroundingsChecker->SetupAttachment(RootComponent);
	SurroundingsChecker->SetRaycastMask(raycastMaskIgnoreActors);
}

//////////////////////////////////////////////////////////////////////////
// Input

void ATestRPGCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	PrimaryActorTick.bCanEverTick = true;

	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ATestRPGCharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("Evade", IE_Pressed, this, &ATestRPGCharacter::StartSlide);
	PlayerInputComponent->BindAction("Evade", IE_Released, this, &ATestRPGCharacter::EndSlide);

	PlayerInputComponent->BindAxis("Move Forward / Backward", this, &ATestRPGCharacter::MoveForward);
	PlayerInputComponent->BindAxis("Move Right / Left", this, &ATestRPGCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn Right / Left Mouse", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("Turn Right / Left Gamepad", this, &ATestRPGCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("Look Up / Down Mouse", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Look Up / Down Gamepad", this, &ATestRPGCharacter::LookUpAtRate);
}

void ATestRPGCharacter::Tick(float deltaSeconds) {

	Super::Tick(deltaSeconds);

	if (ShowDebug) {
		DrawDebugLines();
	}

	//For testing

	FVector raycastRadius = GetActorForwardVector() * 10.0f;

	SurroundingsChecker->GetSurroundings();

	if (ATestRPGCharacter::isSliding) {

		//Draw Forward debug line
		if (GEngine && ShowDebug) {
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, VelocityAsString());
		}

		AdjustSlide();

	}

}

void ATestRPGCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds());
}

void ATestRPGCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds());
}

void ATestRPGCharacter::MoveForward(float Value)
{
	forwardInput = Value;

	if ((Controller != nullptr) && (Value != 0.0f) && !isSliding)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ATestRPGCharacter::MoveRight(float Value)
{
	rightInput = Value;

	if ((Controller != nullptr) && (Value != 0.0f) && !isSliding)
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

//https://www.youtube.com/watch?v=oe2vPXvFLpI
void ATestRPGCharacter::Jump() {

	if (m_jump_curve) {
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Jump curve found, implement custom jump."));
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Jump curve not found on ATestRPGCharacter::Jump()."));
		ACharacter::Jump();
	}
}

void ATestRPGCharacter::StartSlide()
{

	isSliding = true;

	slideDirection = GetActorForwardVector();
	GetCharacterMovement()->MaxAcceleration = 5000.0f;

	if (GEngine && ShowDebug) {
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Slide Start, Slide Direction: " + slideDirection.ToString()) + " Starting Velocity: " + VelocityAsString());
	}

	GetCharacterMovement()->MaxWalkSpeed = baseSlideSpeed;

}

void ATestRPGCharacter::AdjustSlide() {

	float scale = 0.01f;

	if (!slideAccelerationCurve) {
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Slide curve not found on ATestRPGCharacter::AdjustSlide()."));
	}

	float groundAngle = SurroundingsChecker->OrientedGroundAngle;

	//TO DO: MOVE THESE CURVE MULTIPLIERS OUT TO SETABLE VARIABLES
	//Set max acceleration based on slope
	float maxAcceleration = slideAccelerationCurve->FloatCurve.Eval(groundAngle) * 1000;
	GetCharacterMovement()->MaxAcceleration = maxAcceleration;

	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Total Acceleration: " + FString::SanitizeFloat(maxAcceleration)));

	//Both MaxWalkSpeed and MaxAcceleration need to be made slope-dependent
	float maxWalkSpeed = GetCharacterMovement()->MaxWalkSpeed;

	//Slide speed increase
	if (groundAngle <= -1) {
		
		float extraMaxSpeed = maxSlideSpeed - baseSlideSpeed;
		float maxGroundAngle = 45;

		float walkSpeedScalar = baseMaxWalkSpeed + ((groundAngle / maxGroundAngle) * -299.0f) + (groundAngle > 0 ? 1 : -1);

		float currentWalkSpeed = GetCharacterMovement()->MaxWalkSpeed;
		
		if (currentWalkSpeed < maxSlideSpeed)
			GetCharacterMovement()->MaxWalkSpeed = currentWalkSpeed + (-groundAngle / 45) + 0.05;
		else
			GetCharacterMovement()->MaxWalkSpeed = maxSlideSpeed;

	}
	//Slide speed decrease
	else {

		//Gradually decay more as you get slower? idk see what feels right`
		GetCharacterMovement()->MaxWalkSpeed = maxWalkSpeed - 1.5f;
	
	}

	GetCharacterMovement()->AddInputVector(slideDirection);

}

void ATestRPGCharacter::EndSlide()
{
	isSliding = false;
	GetCharacterMovement()->MaxAcceleration = 1500.0f;

	if (GEngine && ShowDebug) {
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Slide End"));
	}

	GetCharacterMovement()->MaxWalkSpeed = baseMaxWalkSpeed;
}

FString ATestRPGCharacter::VelocityAsString() {

	return FString::SanitizeFloat(GetCharacterMovement()->Velocity.Length());
}

void ATestRPGCharacter::DrawDebugLines() {
	FHitResult Hit = FHitResult(ForceInit);
	FVector raycastDistance = FVector(0, 0, 100.0f);

	FVector startPos = GetActorLocation();

	FCollisionQueryParams CollisionParams = FCollisionQueryParams();
	CollisionParams.bTraceComplex = true;
	if (raycastMaskIgnoreActors.Num() > 0) {
		for (auto actor : raycastMaskIgnoreActors) {
			CollisionParams.AddIgnoredActor(actor);
		}
	}

	//Raycast Down
	GetWorld()->LineTraceSingleByChannel(Hit, startPos, startPos - raycastDistance, ECC_WorldDynamic, CollisionParams);

	//Straight down vertical line
	DrawDebugLine(GetWorld(), startPos, startPos - raycastDistance, FColor::Red, false, 0, 0, 1.0f);
	//Normal of ground impact
	DrawDebugLine(GetWorld(), Hit.Location, Hit.Location + (Hit.Normal * 100), FColor::Blue, false, 0, 0, 1.0f);
	//Forward adjustment for ground impact normal
	double groundAngle = SurroundingsChecker->OrientedGroundAngle;
	FVector3d groundRotateAngle = GetActorForwardVector().RotateAngleAxis(groundAngle, -GetActorRightVector());

	//Keep for debugging
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Ground Rotate Angle: " + groundRotateAngle.ToString()));

	DrawDebugLine(GetWorld(), startPos, startPos + groundRotateAngle * 100, FColor::Blue, false, 0, 0, 1.0f);
	//DrawDebugLine(GetWorld(), startPos, startPos + (GetActorForwardVector().RotateAngleAxis(SurroundingsChecker->OrientedGroundAngle, FVector3d::YAxisVector)) * 100, FColor::Blue, false, 0, 0, 1.0f);
	//Forward vector
	DrawDebugLine(GetWorld(), startPos, startPos + (GetActorForwardVector() * 100), FColor::Orange, false, 0, 0, 1.0f);

	//If on a slope
	if (Hit.Normal.Y != 0) {
		//DrawDebugLine(GetWorld(), startPos, startPos)
	}
}