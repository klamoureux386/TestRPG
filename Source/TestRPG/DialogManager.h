// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "DialogManager.generated.h"

USTRUCT()
struct FDialog
{
	GENERATED_BODY()

public:
	FDialog();

	int Id;					//Unique Id for Dialog
	FString Text;			//Content of subtitle
	FString Description;	//5 W's description
	USoundBase* Sound;		//Ptr to sound file
	int Duration;			//Duration of line to be shown as subtitle
	int NextDialogId;		//Dialog to follow after completion of this Dialog

	FDialog(int id, FString text, int duration, int nextDialog)
	{
		Id = id;
		Text = text;
		Duration = duration;
		NextDialogId = nextDialog;
	}

	FDialog(int id, FString text, int duration)
	{
		Id = id;
		Text = text;
		Duration = duration;
		NextDialogId = NULL;
	}
};

UCLASS()
class TESTRPG_API UDialogManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

	public:
		// Sets default values for this component's properties
		UDialogManager();
		FDialog GetDialogById(int32 id);

	protected:
		UPROPERTY(EditAnywhere, Category = "Dialog")
		TMap<int32, FDialog> DialogLookup;

	
};
