// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class TESTRPG_API Dialog
{
public:
	Dialog();
	~Dialog();

	int Id;			//Unique Id for Dialog
	FString Line;		//Content of subtitle
	//Sound
	int Duration;		//Duration of line to be shown as subtitle
	int NextDialogId;	//Dialog to follow after completion of this Dialog

	Dialog(int id, FString line, int duration, int nextDialog)
	{
		Id = id;
		Line = line;
		Duration = duration;
		NextDialogId = nextDialog;
	}

	Dialog(int id, FString line, int duration) 
	{
		Id = id;
		Line = line;
		Duration = duration;
		NextDialogId = NULL;
	}

};
