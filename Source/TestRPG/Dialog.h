// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Dialog.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct TESTRPG_API FDialog
{
	GENERATED_BODY();

public:
	FDialog();
	~FDialog();

	int32 Id;					//Unique Id for Dialog
	FString Text;			//Content of subtitle
	FString Description;	//5 W's description
	USoundBase* Sound;		//Ptr to sound file
	int32 Duration;			//Duration of line to be shown as subtitle
	int32 NextDialogId;		//Dialog to follow after completion of this Dialog

	FDialog(int32 id, FString text, FString description, int32 duration, int32 nextDialog)
	{
		Id = id;
		Text = text;
		Description = description;
		Duration = duration;
		NextDialogId = nextDialog;
	}

	FDialog(int32 id, FString text, FString description, int32 duration)
	{
		Id = id;
		Text = text;
		Description = description;
		Duration = duration;
		NextDialogId = NULL;
	}
};
