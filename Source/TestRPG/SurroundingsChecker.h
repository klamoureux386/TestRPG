// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SurroundingsChecker.generated.h"

UCLASS()
class TESTRPG_API ASurroundingsChecker : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASurroundingsChecker();
	FVector GetGroundNormal(FVector startPos, std::vector<AActor*> actorsToIgnore);
	float GetOrientedGroundAngle(FVector backRaycastStart, FVector frontRaycastStart, std::vector<AActor*> actorsToIgnore);

private:
	void ShowDebugMenu(bool show);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
