// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Dialog.h"
#include "Sound/SoundCue.h"
#include "DialogManager.generated.h"

UCLASS()
class TESTRPG_API UDialogManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

	public:
		// Sets default values for this component's properties
		UDialogManager();
		UFUNCTION(BlueprintCallable)
		FDialog GetDialogById(int32 id);

	private:
		USoundWave* GetAudioFile(FString filepath);

	protected:
		UPROPERTY(EditAnywhere, Category = "Dialog")
		TMap<int32, FDialog> DialogLookup;
};
