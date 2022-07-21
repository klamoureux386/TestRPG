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
	GetCharacterMovement()->MaxWalkSpeed = 700.f;
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

	//Set up Utility components
	SurroundingsChecker = CreateDefaultSubobject<ASurroundingsChecker>(TEXT("SurroundingsChecker"));
	raycastMaskIgnoreActors = std::vector<AActor*>();
	raycastMaskIgnoreActors.push_back(this);

}

//////////////////////////////////////////////////////////////////////////
// Input

void ATestRPGCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	PrimaryActorTick.bCanEverTick = true;

	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
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

	bool debug = true;
	if (debug) {
		DrawDebugLines();
	}

	//For testing

	FVector raycastRadius = GetActorForwardVector() * 10.0f;

	SurroundingsChecker->GetOrientedGroundAngle(GetActorLocation() - raycastRadius, GetActorLocation() + raycastRadius, raycastMaskIgnoreActors);

	if (ATestRPGCharacter::isSliding) {

		//Draw Forward debug line
		if (GEngine) {
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, VelocityAsString());
		}
		const float scale = 1.001f;

		GetGroundNormal();

		AddMovementInput(slideDirection, scale);
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
		GetCharacterMovement()->MaxAcceleration = -10.0f;
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

void ATestRPGCharacter::StartSlide()
{

	isSliding = true;

	slideDirection = GetActorForwardVector();
	GetCharacterMovement()->MaxAcceleration = 500.0f;

	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Slide Start, Slide Direction: " + slideDirection.ToString()) + " Starting Velocity: " + VelocityAsString());
		GetCharacterMovement()->MaxWalkSpeed = 1000.0f;
	}

}

void ATestRPGCharacter::EndSlide()
{
	isSliding = false;
	GetCharacterMovement()->MaxAcceleration = 1500.0f;

	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Slide End"));
		GetCharacterMovement()->MaxWalkSpeed = 500.0f;
	}
}

FString ATestRPGCharacter::VelocityAsString() {

	return FString::SanitizeFloat(GetCharacterMovement()->Velocity.Length());
}

FVector ATestRPGCharacter::GetGroundNormal() {

	FVector Normal = SurroundingsChecker->GetGroundNormal(GetActorLocation(), raycastMaskIgnoreActors);

	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, "Ground normal: " + Normal.ToString());
	}


	return Normal;
}

void ATestRPGCharacter::DrawDebugLines() {
	FHitResult Hit = FHitResult(ForceInit);
	FVector raycastDistance = FVector(0, 0, 100.0f);

	FVector startPos = GetActorLocation();

	FCollisionQueryParams CollisionParams = FCollisionQueryParams();
	CollisionParams.bTraceComplex = true;
	if (raycastMaskIgnoreActors.size() > 0) {
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
	//Forward vector
	DrawDebugLine(GetWorld(), startPos, startPos + (GetActorForwardVector() * 100), FColor::Orange, false, 0, 0, 1.0f);

	//If on a slope
	if (Hit.Normal.Y != 0) {
		//DrawDebugLine(GetWorld(), startPos, startPos)
	}
}