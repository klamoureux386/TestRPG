// Fill out your copyright notice in the Description page of Project Settings.


#include "SurroundingsChecker.h"
#include "Kismet/KismetMathLibrary.h"

USurroundingsChecker::USurroundingsChecker()
{
	
}	

void USurroundingsChecker::SetRaycastMask(TArray<AActor*> raycastMaskIgnoreActors) {
	USurroundingsChecker::actorsToIgnore = raycastMaskIgnoreActors;
	DrawDebug(true);
}

// Called when the game starts or when spawned
void USurroundingsChecker::BeginPlay()
{
	Super::BeginPlay();
	ActorCenter = GetAttachParent()->GetRelativeTransform();
}

void USurroundingsChecker::GetSurroundings() 
{
	ActorCenter = GetAttachParent()->GetRelativeTransform();
	SetGroundNormal();
	SetOrientedGroundAngle();
}

//https://forums.unrealengine.com/t/c-how-to-ignore-the-character-actor-while-line-tracing/354776/3
void USurroundingsChecker::SetGroundNormal() {

	FHitResult Hit = FHitResult(ForceInit);
	FVector raycastDistance = FVector(0, 0, 100.0f);

	FVector startPos = ActorCenter.GetLocation();

	FCollisionQueryParams CollisionParams = FCollisionQueryParams();
	CollisionParams.bTraceComplex = true;

	if (actorsToIgnore.Num() > 0) {
		for (auto actor : actorsToIgnore) {
			CollisionParams.AddIgnoredActor(actor);
		}
	}

	GetWorld()->LineTraceSingleByChannel(Hit, startPos, startPos - raycastDistance, ECC_WorldDynamic, CollisionParams);

	if (ShowDebug)
	{
		//InitialRaycast
		DrawDebugLine(GetWorld(), startPos, startPos - raycastDistance, FColor::Red, false, 0, 0, 1.0f);
		DrawDebugLine(GetWorld(), Hit.Location, Hit.Location + (Hit.Normal * 100), FColor::Blue, false, 0, 0, 1.0f);

		//Debug message to ensure raycast masking works
		if (GEngine) {
			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, Hit.GetComponent() != nullptr ? Hit.GetComponent()->GetName() : "No name found");
		}
	}

	GroundNormal = Hit.Normal;

}

void USurroundingsChecker::SetOrientedGroundAngle() {

	FHitResult BackHit = FHitResult(ForceInit);
	FHitResult FrontHit = FHitResult(ForceInit);
	FVector raycastDistance = FVector(0, 0, 125.0f);

	FVector backRaycastStart = BackRaycastPos();
	FVector frontRaycastStart = FrontRaycastPos();

	//Debug message for raycast positions
	if (GEngine && ShowDebug) {
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, "backRaycastStart: " + BackRaycastPos().ToString());
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, "ActorCenter: " + ActorCenter.GetLocation().ToString());
	}

	FCollisionQueryParams CollisionParams = FCollisionQueryParams();
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
		//Debug message to ensure raycast masking works
		if (GEngine && ShowDebug) {
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, "orientedGroundAngle: " + FString::SanitizeFloat(angle));
		}

		OrientedGroundAngle = angle;
	}
	else {
		OrientedGroundAngle = 0;
	}
	

	if (ShowDebug) {
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
		}

	}

}

void USurroundingsChecker::DrawDebug(bool show) {

	if (!show)
		return;

	//to do: implement
}