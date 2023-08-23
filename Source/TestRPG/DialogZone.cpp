// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogZone.h"

// Sets default values
ADialogZone::ADialogZone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	CollisionBox->SetBoxExtent(FVector(64.f, 64.f, 64.f));
	CollisionBox->SetCollisionProfileName("Trigger");
	RootComponent = CollisionBox;

	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ADialogZone::OnOverlapBegin);
	CollisionBox->OnComponentEndOverlap.AddDynamic(this, &ADialogZone::OnOverlapEnd);

}

// Called when the game starts or when spawned
void ADialogZone::BeginPlay()
{
	Super::BeginPlay();

	DialogManager = GetWorld()->GetGameInstance()->GetSubsystem<UDialogManager>();
	
}

// Called every frame
void ADialogZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADialogZone::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	FDialog dialog = DialogManager->GetDialogById(1);

	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, dialog.Text);
}

void ADialogZone::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

