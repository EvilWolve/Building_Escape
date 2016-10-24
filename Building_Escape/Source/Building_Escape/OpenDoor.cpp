// Fill out your copyright notice in the Description page of Project Settings.

#include "Building_Escape.h"
#include "OpenDoor.h"

#define OUT

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	Owner = GetOwner();

	if (PressurePlate == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Door %s does not have a pressure plate assigned!"), *Owner->GetName())
	}
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (GetTotalMassOfActorsOnPlate() >= WeightNeededToTrigger)
	{
		//Owner->SetActorRotation(FRotator(0.f, OpenAngle, 0.f));
		OnOpen.Broadcast();

		LastDoorOpenTime = GetWorld()->GetTimeSeconds();
	}

	if (GetWorld()->GetTimeSeconds() - LastDoorOpenTime > DoorCloseDelay)
	{
		//Owner->SetActorRotation(FRotator(0.f, 0.f, 0.f));
		OnClose.Broadcast();
	}
}

float UOpenDoor::GetTotalMassOfActorsOnPlate()
{
	if (PressurePlate == nullptr)
	{
		return 0.f;
	}

	float totalMass = 0.f;

	TArray<AActor*> overlappingActors;
	PressurePlate->GetOverlappingActors(OUT overlappingActors);

	for (AActor* actor : overlappingActors)
	{
		if (actor != nullptr)
		{
			float mass = actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
			totalMass += mass;

			UE_LOG(LogTemp, Warning, TEXT("%s on pressure plate"), *actor->GetName())
		}
	}

	return totalMass;
}
