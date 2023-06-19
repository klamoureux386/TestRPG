// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Dialog.h"

/**
 * 
 */
class TESTRPG_API DialogManager : public UGameInstanceSubsystem
{
public:
	DialogManager();
	~DialogManager();

	Dialog GetDialogById(int id);

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dialog")
	TMap<int32, Dialog> DialogLookup;
};
