// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RoamingLocation.h"
#include "RoamingComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TESTRPG_API URoamingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	URoamingComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC_Roaming")
	bool IsRoaming;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC_Roaming")
	TArray<FRoamingLocation> RoamingLocations;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
