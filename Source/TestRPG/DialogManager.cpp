// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogManager.h"

// Sets default values for this component's properties
UDialogManager::UDialogManager()
{
	TArray<FDialog> dialogs =
	{
		FDialog(1, "Dialog1", "Character1 Dialog", 50, 2),
		FDialog(2, "Dialog2", "Character2 Dialog", 50, 3)
	};

	for (FDialog d : dialogs)
	{
		DialogLookup.Add(d.Id, d);
	}
}


/// <summary> Returns Dialog associated with Id. </summary>
FDialog UDialogManager::GetDialogById(int32 id)
{
	FDialog dialog = DialogLookup.FindRef(id);

	if (dialog.Id == 0)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Unable to find Dialog with Id " + FString::FromInt(id)));

	return dialog;
}