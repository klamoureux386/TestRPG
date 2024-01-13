// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NPCState.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct TESTRPG_API FNPCState
{
	GENERATED_USTRUCT_BODY();

public:
	FNPCState();
	~FNPCState();

	int32 CharacterId;		//Unique Id for Character
	int32 DialogId;			//Unique Id for current Dialog

	FNPCState(int32 characterId, int32 dialogId)
	{
		CharacterId = characterId;
		DialogId = dialogId;
	}

};