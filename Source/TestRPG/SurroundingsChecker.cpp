// Fill out your copyright notice in the Description page of Project Settings.


#include "SurroundingsChecker.h"

// Sets default values
ASurroundingsChecker::ASurroundingsChecker()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ShowDebugMenu(true);

}

// Called when the game starts or when spawned
void ASurroundingsChecker::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ASurroundingsChecker::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//https://forums.unrealengine.com/t/c-how-to-ignore-the-character-actor-while-line-tracing/354776/3
FVector ASurroundingsChecker::GetGroundNormal(FVector startPos, std::vector<AActor*> actorsToIgnore = std::vector<AActor*>()) {

	FHitResult Hit = FHitResult(ForceInit);
	FVector raycastDistance = FVector(0, 0, 100.0f);

	FCollisionQueryParams CollisionParams = FCollisionQueryParams();
	CollisionParams.bTraceComplex = true;

	if (actorsToIgnore.size() > 0) {
		for (auto actor : actorsToIgnore) {
			CollisionParams.AddIgnoredActor(actor);
		}
	}

	GetWorld()->LineTraceSingleByChannel(Hit, startPos, startPos - raycastDistance, ECC_WorldDynamic, CollisionParams);

	bool debug = true;
	if (debug)
	{
		//InitialRaycast
		//DrawDebugLine(GetWorld(), startPos, startPos - raycastDistance, FColor::Red, false, 0, 0, 1.0f);
		//DrawDebugLine(GetWorld(), Hit.Location, Hit.Location + (Hit.Normal * 100), FColor::Blue, false, 0, 0, 1.0f);

		//Debug message to ensure raycast masking works
		if (GEngine) {
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, Hit.GetComponent() != nullptr ? Hit.GetComponent()->GetName() : "No name found");
		}
	}

	return Hit.Normal;
}

float ASurroundingsChecker::GetOrientedGroundAngle(FVector backRaycastStart, FVector frontRaycastStart, std::vector<AActor*> actorsToIgnore = std::vector<AActor*>()) {

	FHitResult BackHit = FHitResult(ForceInit);
	FHitResult FrontHit = FHitResult(ForceInit);
	FVector raycastDistance = FVector(0, 0, 110.0f);

	FCollisionQueryParams CollisionParams = FCollisionQueryParams();
	CollisionParams.bTraceComplex = true;

	CollisionParams.bTraceComplex = true;

	if (actorsToIgnore.size() > 0) {
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
			DrawDebugLine(GetWorld(), BackHit.Location + heightIncreaseForLineVisiblity, FrontHit.Location + heightIncreaseForLineVisiblity, FColor::Purple, false, 0, 0, 1.0f);
		}
	}

	return 0.0f;

}

void ASurroundingsChecker::ShowDebugMenu(bool show) {

	if (!show)
		return;

	//to do: implement
}