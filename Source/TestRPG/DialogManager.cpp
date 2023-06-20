// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogManager.h"

// Sets default values for this component's properties
UDialogManager::UDialogManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	TArray<FDialog> dialogs =
	{
		FDialog(1, "a", 50, 2),
		FDialog(2, "b", 50, 3)
	};

	for (FDialog d : dialogs)
	{
		DialogLookup.Add(d.Id, d);
	}
}


// Called when the game starts
void UDialogManager::BeginPlay()
{
	Super::BeginPlay();

	GetDialogById(3);
	
}


// Called every frame
void UDialogManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}


FDialog::FDialog()
{
}


FDialog UDialogManager::GetDialogById(int32 id)
{
	FDialog* dialog = DialogLookup.Find(id);

	if (dialog == nullptr)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Unable to find Dialog with Id " + FString::FromInt(id)));

	return dialog != nullptr ? *dialog : FDialog();

}