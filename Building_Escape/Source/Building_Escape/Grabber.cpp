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
}

// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	FindPhysicsHandleComponent();
	SetupInputComponent();
}

void UGrabber::FindPhysicsHandleComponent()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();

	if (PhysicsHandle == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("PhysicsHandle component not found on actor: %s"), *GetOwner()->GetName())
	}
}

void UGrabber::SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();

	if (InputComponent)
	{
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Input component not found on actor: %s"), *GetOwner()->GetName())
	}
}

// Called every frame
void UGrabber::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	if (PhysicsHandle == nullptr)
	{
		return;
	}

	if (PhysicsHandle->GrabbedComponent)
	{
		FVector lineTraceEnd = GetLineTraceEnd();

		PhysicsHandle->SetTargetLocation(lineTraceEnd);
	}
}

void UGrabber::Grab()
{
	FHitResult hit = GetFirstPhysicsBodyInReach();
	AActor* hitActor = hit.GetActor();
	if (hitActor)
	{
		UPrimitiveComponent* componentToGrab = hit.GetComponent();
		PhysicsHandle->GrabComponent(componentToGrab, NAME_None, hitActor->GetActorLocation(), true);
	}
}

void UGrabber::Release()
{
	PhysicsHandle->ReleaseComponent();
}

const FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	FVector lineTraceStart = GetLineTraceStart();
	FVector lineTraceEnd = GetLineTraceEnd();

	FHitResult hit;
	FCollisionQueryParams traceParams(FName(TEXT("")), false, GetOwner());

	GetWorld()->LineTraceSingleByObjectType(
		OUT hit,
		lineTraceStart,
		lineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		traceParams
	);

	return hit;
}

const FVector UGrabber::GetLineTraceStart()
{
	FVector playerViewPointPosition;
	FRotator playerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT playerViewPointPosition, OUT playerViewPointRotation);

	return playerViewPointPosition;
}

const FVector UGrabber::GetLineTraceEnd()
{
	FVector playerViewPointPosition;
	FRotator playerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT playerViewPointPosition, OUT playerViewPointRotation);

	return playerViewPointPosition + playerViewPointRotation.Vector() * Reach;
}
