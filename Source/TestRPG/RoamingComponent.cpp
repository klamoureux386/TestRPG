// Fill out your copyright notice in the Description page of Project Settings.


#include "RoamingComponent.h"

// Sets default values for this component's properties
URoamingComponent::URoamingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void URoamingComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

	for (int i = 0; i < RoamingLocations.Num(); i++)
	{
		FRoamingLocation var = RoamingLocations[i];

		if (GEngine) {
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, "roaming location: " + var.Location.ToString());
		}

		DrawDebugSphere(GetWorld(), var.Location, 100, 12, FColor::Green, true, 1000.0f);
	}
	
}


// Called every frame
void URoamingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

