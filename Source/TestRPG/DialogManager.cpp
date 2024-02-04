// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogManager.h"

// Sets default values for this component's properties
UDialogManager::UDialogManager()
{
	//"/Game/Audio/Dialog/_AI_WhatCanIGetForYou"
	// Load our Sound Cue for the propeller sound we created in the editor... note your path may be different depending
	// on where you store the asset on disk.
	static ConstructorHelpers::FObjectFinder<USoundWave> propellerCue2(TEXT("/Game/Audio/Dialog/_AI_WhatCanIGetForYou"));
	
	TArray<FDialog> dialogs =
	{
		FDialog(
			1,
			"What can I do for you?",
			"Character1 Dialog",
			0.0f,
			{ 2, 3, 4 },
			2,
			GetAudioFile("/Game/Audio/Dialog/_AI_WhatCanIGetForYou")
		),
		//Response options
		FDialog(2, "Test out my dialog #2.", "Dialog1Response1", 1.0f, propellerCue2.Object),
		FDialog(3, "Test out my dialog #3.", "Dialog1Response2", 1.0f, GetAudioFile("/Game/Audio/Dialog/_AI_WhatCanIGetForYou")),
		FDialog(4, "Test out my dialog #4.", "Dialog1Response3", 1.0f, GetAudioFile("/Game/Audio/Dialog/_AI_WhatCanIGetForYou")),
		//End response options
		FDialog(5, "Dialog5", "Character2 Dialog", 1.0f, 3)
	};

	for (FDialog d : dialogs)
	{
		DialogLookup.Add(d.Id, d);
	}
	
}

USoundWave* UDialogManager::GetAudioFile(FString filepath) {

	//"/Game/Audio/Dialog/_AI_WhatCanIGetForYou"
	static ConstructorHelpers::FObjectFinder<USoundWave> propellerCue(*(filepath));

	return propellerCue.Object;
}

/// <summary> Returns Dialog associated with Id. </summary>
FDialog UDialogManager::GetDialogById(int32 id)
{
	FDialog dialog = DialogLookup.FindRef(id);

	if (dialog.Id == 0)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Unable to find Dialog with Id " + FString::FromInt(id)));

	return dialog;
}