// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Engine/EngineTypes.h"
#include "Components/SceneComponent.h"
#include "SurroundingsChecker.generated.h"

UCLASS()
class TESTRPG_API USurroundingsChecker : public USceneComponent
{
	GENERATED_BODY()

	UPROPERTY(Category = "Utility", EditAnywhere, BlueprintReadWrite, meta = (MakeEditWidget=true, AllowPrivateAccess = true))
	FTransform ActorCenter;

public:

	//TO DO: Properly handle actor forward direction when calculating front/back raycast pos

	UFUNCTION(Category = "Utility", BlueprintCallable)
	FVector FrontRaycastPos() { return ActorCenter.GetLocation() + (FrontRaycastOffset /** GetOwner()->GetActorForwardVector()*/); }

	UFUNCTION(Category = "Utility", BlueprintCallable)
	FVector BackRaycastPos() { return ActorCenter.GetLocation() + (BackRaycastOffset /** GetOwner()->GetActorForwardVector()*/); }

	/** Relative Ground Angle */
	UPROPERTY(Category = "Utility", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	double OrientedGroundAngle;

	/** Show Debug */
	UPROPERTY(Category = "Debug", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool ShowDebug = true;

	USurroundingsChecker();
	void GetSurroundings();
	FVector GroundNormal;
	// Sets default values for this actor's properties
	void SetRaycastMask(TArray<AActor*> _raycastMaskIgnoreActors);

private:

	UPROPERTY(Category = "Utility", VisibleAnywhere, BlueprintReadOnly, meta = (MakeEditWidget = true, AllowPrivateAccess = true))
	FVector FrontRaycastOffset = FVector(10, 0, 0);
	UPROPERTY(Category = "Utility", VisibleAnywhere, BlueprintReadOnly, meta = (MakeEditWidget = true, AllowPrivateAccess = true))
	FVector BackRaycastOffset = FVector(-10, 0, 0);

	TArray<AActor*> actorsToIgnore;
	void SetGroundNormal();
	void SetOrientedGroundAngle();
	void DrawDebug(bool show);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	//virtual void Tick(float DeltaTime) override;

};
