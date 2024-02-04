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

	//Unique Id for Dialog
	UPROPERTY(BlueprintReadOnly)
	int32 Id;

	//Content of subtitle
	UPROPERTY(BlueprintReadOnly)
	FString Text = "";

	//5 W's description
	UPROPERTY(BlueprintReadOnly)
	FString Description = "";

	//Duration of line to be shown as subtitle in seconds
	UPROPERTY(BlueprintReadOnly)
	float Duration = 0;

	//Optional array of dialog response option ids
	UPROPERTY(BlueprintReadOnly)
	TArray<int32> ResponseOptions;

	//Dialog to follow after completion of this Dialog
	UPROPERTY(BlueprintReadOnly)
	int32 NextDialogId;

	//Ptr to sound file
	UPROPERTY(BlueprintReadOnly, Category = "Audio")
	USoundBase* Sound;

	FDialog(int32 id, FString text, FString description, float duration, TArray<int32> responseOptions, int32 nextDialog, USoundBase* sound)
	{
		Id = id;
		Text = text;
		Description = description;
		Duration = duration;
		ResponseOptions = responseOptions;
		NextDialogId = nextDialog;
		Sound = sound;
	}

	FDialog(int32 id, FString text, FString description, float duration, TArray<int32> responseOptions, int32 nextDialog)
	{
		Id = id;
		Text = text;
		Description = description;
		Duration = duration;
		ResponseOptions = responseOptions;
		NextDialogId = nextDialog;
		Sound = nullptr;
	}

	FDialog(int32 id, FString text, FString description, float duration, int32 nextDialog, USoundBase* sound)
	{
		Id = id;
		Text = text;
		Description = description;
		Duration = duration;
		NextDialogId = nextDialog;
		Sound = sound;
	}

	FDialog(int32 id, FString text, FString description, float duration, int32 nextDialog)
	{
		Id = id;
		Text = text;
		Description = description;
		Duration = duration;
		NextDialogId = nextDialog;
		Sound = nullptr;
	}

	FDialog(int32 id, FString text, FString description, float duration, USoundBase* sound)
	{
		Id = id;
		Text = text;
		Description = description;
		Duration = duration;
		Sound = sound;
	}

	FDialog(int32 id, FString text, FString description, float duration)
	{
		Id = id;
		Text = text;
		Description = description;
		Duration = duration;
		Sound = nullptr;
	}
};
