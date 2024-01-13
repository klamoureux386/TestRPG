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
	GENERATED_USTRUCT_BODY();

public:
	FDialog();
	~FDialog();

	UPROPERTY(BlueprintReadOnly)
	int32 Id;						//Unique Id for Dialog
	UPROPERTY(BlueprintReadOnly)
	FString Text;					//Content of subtitle
	UPROPERTY(BlueprintReadOnly)
	FString Description;			//5 W's description
	USoundBase* Sound;				//Ptr to sound file
	UPROPERTY(BlueprintReadOnly)
	int32 Duration;					//Duration of line to be shown as subtitle
	UPROPERTY(BlueprintReadOnly)
	TArray<int32> ResponseOptions;	//Optional array of dialog response option ids
	UPROPERTY(BlueprintReadOnly)
	int32 NextDialogId;				//Dialog to follow after completion of this Dialog

	FDialog(int32 id, FString text, FString description, int32 duration, TArray<int32> responseOptions, int32 nextDialog)
	{
		Id = id;
		Text = text;
		Description = description;
		Duration = duration;
		ResponseOptions = responseOptions;
		NextDialogId = nextDialog;
	}

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
	}
};
