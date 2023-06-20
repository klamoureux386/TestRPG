// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DialogManager.generated.h"

USTRUCT()
struct FDialog
{
	GENERATED_USTRUCT_BODY()

public:
	FDialog();

	int Id;			//Unique Id for Dialog
	FString Line;		//Content of subtitle
	//Sound
	int Duration;		//Duration of line to be shown as subtitle
	int NextDialogId;	//Dialog to follow after completion of this Dialog

	FDialog(int id, FString line, int duration, int nextDialog)
	{
		Id = id;
		Line = line;
		Duration = duration;
		NextDialogId = nextDialog;
	}

	FDialog(int id, FString line, int duration)
	{
		Id = id;
		Line = line;
		Duration = duration;
		NextDialogId = NULL;
	}
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TESTRPG_API UDialogManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDialogManager();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere, Category = "Dialog")
	TMap<int32, FDialog> DialogLookup;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	FDialog GetDialogById(int32 id);
		
};
