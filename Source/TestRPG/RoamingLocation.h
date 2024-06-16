// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RoamingLocation.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct TESTRPG_API FRoamingLocation
{
	GENERATED_BODY()
public:
	FRoamingLocation();
	~FRoamingLocation();

	//World Location
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC_Roaming")
	FVector Location;

	//Acceptable radius offset to navigate to location for variety.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC_Roaming")
	int32 Radius = 0.0;

	FRoamingLocation(FVector location, int32 radius) 
	{
		Location = location;
		Radius = radius;
	}
};
