// Fill out your copyright notice in the Description page of Project Settings.


#include "SurroundingsChecker.h"
#include "Kismet/KismetMathLibrary.h"

USurroundingsChecker::USurroundingsChecker()
{
	ActorCenter = CreateDefaultSubobject<USceneComponent>(TEXT("ActorCenter"));
	ActorCenter->SetRelativeLocation(FVector(0, 0, 0));
	FrontRaycastPos = CreateDefaultSubobject<USceneComponent>(TEXT("FrontRaycastPos"));
	FrontRaycastPos->SetRelativeLocation(FVector(10, 0, 0));
	BackRaycastPos = CreateDefaultSubobject<USceneComponent>(TEXT("BackRaycastPos"));
	BackRaycastPos->SetRelativeLocation(FVector(-10, 0, 0));
}

void USurroundingsChecker::SetRaycasts(TArray<AActor*> raycastMaskIgnoreActors) {
	USurroundingsChecker::actorsToIgnore = raycastMaskIgnoreActors;
	//ActorCenter->SetRelativeLocation(actorLocation);
	//FrontRaycastPos->SetRelativeLocation(actorLocation + actorForward * radiusBetweenRaycasts);
	//BackRaycastPos->SetRelativeLocation(actorLocation - actorForward * radiusBetweenRaycasts);

	DrawDebug(true);
}

// Called when the game starts or when spawned
void USurroundingsChecker::BeginPlay()
{
	Super::BeginPlay();

}

//https://forums.unrealengine.com/t/c-how-to-ignore-the-character-actor-while-line-tracing/354776/3
FVector USurroundingsChecker::GetGroundNormal() {

	FHitResult Hit = FHitResult(ForceInit);
	FVector raycastDistance = FVector(0, 0, 100.0f);

	FVector startPos = ActorCenter->GetComponentLocation();

	FCollisionQueryParams CollisionParams = FCollisionQueryParams();
	CollisionParams.bTraceComplex = true;

	if (actorsToIgnore.Num() > 0) {
		for (auto actor : actorsToIgnore) {
			CollisionParams.AddIgnoredActor(actor);
		}
	}

	GetWorld()->LineTraceSingleByChannel(Hit, startPos, startPos - raycastDistance, ECC_WorldDynamic, CollisionParams);

	bool debug = true;
	if (debug)
	{
		//InitialRaycast
		DrawDebugLine(GetWorld(), startPos, startPos - raycastDistance, FColor::Red, false, 0, 0, 1.0f);
		DrawDebugLine(GetWorld(), Hit.Location, Hit.Location + (Hit.Normal * 100), FColor::Blue, false, 0, 0, 1.0f);

		//Debug message to ensure raycast masking works
		if (GEngine) {
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, Hit.GetComponent() != nullptr ? Hit.GetComponent()->GetName() : "No name found");
		}
	}

	return Hit.Normal;
}

void USurroundingsChecker::SetGroundAngle() {

	FHitResult BackHit = FHitResult(ForceInit);
	FHitResult FrontHit = FHitResult(ForceInit);
	FVector raycastDistance = FVector(0, 0, 110.0f);

	FVector backRaycastStart = BackRaycastPos->GetComponentLocation();
	FVector frontRaycastStart = FrontRaycastPos->GetComponentLocation();

	FCollisionQueryParams CollisionParams = FCollisionQueryParams();
	CollisionParams.bTraceComplex = true;

	CollisionParams.bTraceComplex = true;

	if (actorsToIgnore.Num() > 0) {
		for (auto actor : actorsToIgnore) {
			CollisionParams.AddIgnoredActor(actor);
		}
	}

	GetWorld()->LineTraceSingleByChannel(BackHit, backRaycastStart, backRaycastStart - raycastDistance, ECC_WorldDynamic, CollisionParams);
	GetWorld()->LineTraceSingleByChannel(FrontHit, frontRaycastStart, frontRaycastStart - raycastDistance, ECC_WorldDynamic, CollisionParams);

	if (BackHit.IsValidBlockingHit() && FrontHit.IsValidBlockingHit()) {
		FVector flatLineForward = BackHit.Location.ForwardVector;
		FVector flatLineAngled = FrontHit.Location - BackHit.Location;

		FRotator rotation = UKismetMathLibrary::FindLookAtRotation(flatLineForward, flatLineAngled);
		double angle = rotation.Pitch;

		RelativeGroundAngle = rotation.Pitch;
	}

}

float USurroundingsChecker::GetOrientedGroundAngle() {

	FHitResult BackHit = FHitResult(ForceInit);
	FHitResult FrontHit = FHitResult(ForceInit);
	FVector raycastDistance = FVector(0, 0, 110.0f);

	FVector backRaycastStart = BackRaycastPos->GetComponentLocation();
	FVector frontRaycastStart = FrontRaycastPos->GetComponentLocation();

	//Debug message for raycast positions
	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, "SurroundingsCheckerPos: " + GetComponentLocation().ToString());
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, "backRaycastStart: " + backRaycastStart.ToString());
	}

	FCollisionQueryParams CollisionParams = FCollisionQueryParams();
	CollisionParams.bTraceComplex = true;

	CollisionParams.bTraceComplex = true;

	if (actorsToIgnore.Num() > 0) {
		for (auto actor : actorsToIgnore) {
			CollisionParams.AddIgnoredActor(actor);
		}
	}

	GetWorld()->LineTraceSingleByChannel(BackHit, backRaycastStart, backRaycastStart - raycastDistance, ECC_WorldDynamic, CollisionParams);
	GetWorld()->LineTraceSingleByChannel(FrontHit, frontRaycastStart, frontRaycastStart - raycastDistance, ECC_WorldDynamic, CollisionParams);

	bool debug = true;
	if (debug) {
		//Draw raycast source position purple lines
		DrawDebugLine(GetWorld(), backRaycastStart, backRaycastStart - raycastDistance, FColor::Purple, false, 0, 0, 1.0f);
		DrawDebugLine(GetWorld(), frontRaycastStart, frontRaycastStart - raycastDistance, FColor::Purple, false, 0, 0, 1.0f);

		if (BackHit.IsValidBlockingHit()) {
			//Draw raycast debug sphere at impact
			DrawDebugSphere(GetWorld(), BackHit.Location, 1, 12, FColor::Green);
		}

		if (FrontHit.IsValidBlockingHit()) {
			//Draw raycast debug sphere at impact
			DrawDebugSphere(GetWorld(), FrontHit.Location, 1, 12, FColor::Green);
		}

		if (BackHit.IsValidBlockingHit() && FrontHit.IsValidBlockingHit()) {
			//Draw slope average purple line
			FVector heightIncreaseForLineVisiblity = FVector(0.0f, 0.0f, 1.0f);
			DrawDebugLine(GetWorld(), BackHit.Location + heightIncreaseForLineVisiblity, FrontHit.Location + heightIncreaseForLineVisiblity, FColor::Blue, false, 0, 0, 1.0f);
			
			FVector flatLineForward = BackHit.Location.ForwardVector;
			FVector flatLineAngled = FrontHit.Location - BackHit.Location;

			FRotator rotation = UKismetMathLibrary::FindLookAtRotation(flatLineForward, flatLineAngled);
			double angle = rotation.Pitch;
			//Debug message to ensure raycast masking works
			if (GEngine) {
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, "groundAngle: " + FString::SanitizeFloat(angle));
			}

		}
	}

	return 0.0f;

}

void USurroundingsChecker::DrawDebug(bool show) {

	if (!show)
		return;

	//to do: implement
}