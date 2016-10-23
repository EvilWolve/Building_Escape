// Fill out your copyright notice in the Description page of Project Settings.

#include "Building_Escape.h"
#include "Grabber.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UGrabber::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	FVector playerViewPointPosition;
	FRotator playerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT playerViewPointPosition, OUT playerViewPointRotation);

	//UE_LOG(LogTemp, Warning, TEXT("Location: %s\t\tRotation: %s"), *playerViewPointPosition.ToString(), *playerViewPointRotation.ToString())

	FVector lineTraceEnd = playerViewPointPosition + playerViewPointRotation.Vector() * Reach;

	DrawDebugLine(
		GetWorld(),
		playerViewPointPosition,
		lineTraceEnd,
		FColor(255, 0, 0, 255),
		false,
		-1.f,
		0,
		10.f
	);

	FHitResult hit;
	FCollisionQueryParams traceParams(FName(TEXT("")), false, GetOwner());

	GetWorld()->LineTraceSingleByObjectType(
		OUT hit,
		playerViewPointPosition,
		lineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		traceParams
	);

	AActor* hitActor = hit.GetActor();
	if (hitActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("Hit actor: %s"), *hitActor->GetName())
	}
}

