// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogManager.h"

// Sets default values for this component's properties
UDialogManager::UDialogManager()
{
	TArray<int32> options = { 1, 2, 3 };

	TArray<FDialog> dialogs =
	{
		FDialog(1, "What can I do for you?", "Character1 Dialog", 50, options, 2),
		//Response options
		FDialog(2, "Test out my dialog #2.", "Dialog1Response1", 3),
		FDialog(3, "Test out my dialog #3.", "Dialog1Response2", 3),
		FDialog(4, "Test out my dialog #4.", "Dialog1Response3", 3),
		//End response options
		FDialog(5, "Dialog5", "Character2 Dialog", 50, 3)
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