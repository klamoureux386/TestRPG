// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogManager.h"

DialogManager::DialogManager()
{
	TArray<Dialog> dialogs =
	{
		Dialog(1, "a", 50, 2),
		Dialog(2, "b", 50, 3)
	};

	for (Dialog d : dialogs) 
	{
		DialogLookup.Add(d.Id, d);
	}

	GetDialogById(3);

}

DialogManager::~DialogManager()
{
}

Dialog DialogManager::GetDialogById(int32 id) 
{
	Dialog* dialog = DialogLookup.Find(id);

	if (dialog == nullptr)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Unable to find Dialog with Id " + id));

	return dialog != nullptr ? *dialog : Dialog();

}