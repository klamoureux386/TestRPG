// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "NPCState.h"
#include "NPCStateManager.generated.h"

/**
 * 
 */
UCLASS()
class TESTRPG_API UNPCStateManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Dialog")
	TMap<int32, FNPCState> stateLookup;

	int32 GetDialogIdForCharacter(int32 characterId) {

		FNPCState state = stateLookup.FindRef(characterId);

		return state.DialogId;
	}
	
};
