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

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Utility, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* ActorCenter;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Utility, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* FrontRaycastPos;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Utility, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* BackRaycastPos;

	/** Relative Ground Angle */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Utility, meta = (AllowPrivateAccess = "true"))
	float RelativeGroundAngle;

public:

	USurroundingsChecker();
	void GetSurroundings();
	double OrientedGroundAngle;
	FVector GroundNormal;
	// Sets default values for this actor's properties
	void SetRaycastMask(TArray<AActor*> _raycastMaskIgnoreActors);

private:
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
