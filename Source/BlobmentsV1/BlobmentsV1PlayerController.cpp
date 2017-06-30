// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "BlobmentsV1.h"
#include "BlobmentsV1PlayerController.h"
#include "AI/Navigation/NavigationSystem.h"
#include "Runtime/Engine/Classes/Components/DecalComponent.h"
#include "Kismet/HeadMountedDisplayFunctionLibrary.h"
#include "BlobmentsV1Character.h"

ABlobmentsV1PlayerController::ABlobmentsV1PlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;
}

void ABlobmentsV1PlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	// keep updating the destination every tick while desired
	if (bMoveToMouseCursor)
	{
		MoveToMouseCursor();
	}
}

void ABlobmentsV1PlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	InputComponent->BindAction("SetDestination", IE_Pressed, this, &ABlobmentsV1PlayerController::OnSetDestinationPressed);
	InputComponent->BindAction("SetDestination", IE_Released, this, &ABlobmentsV1PlayerController::OnSetDestinationReleased);

	InputComponent->BindAction("Cancel", IE_Pressed, this, &ABlobmentsV1PlayerController::OnCancel);
	InputComponent->BindAction("Cancel", IE_Released, this, &ABlobmentsV1PlayerController::OnCancel);

	// support touch devices 
	/*InputComponent->BindTouch(EInputEvent::IE_Pressed, this, &ABlobmentsV1PlayerController::MoveToTouchLocation);
	InputComponent->BindTouch(EInputEvent::IE_Released, this, &ABlobmentsV1PlayerController::OnSetDestinationReleased);
	InputComponent->BindTouch(EInputEvent::IE_Repeat, this, &ABlobmentsV1PlayerController::OnCancel);
	*/
	//InputComponent->BindAction("ResetVR", IE_Pressed, this, &ABlobmentsV1PlayerController::OnResetVR);
}

void ABlobmentsV1PlayerController::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

//every tick
void ABlobmentsV1PlayerController::MoveToMouseCursor()  
{
	// Trace to see what is under the mouse cursor
	FHitResult Hit;
	GetHitResultUnderCursor(ECC_Visibility, false, Hit);

	if (Hit.bBlockingHit)
	{
		// We hit something, move there
		SetNewMoveDestination(Hit.ImpactPoint);
	}
}


//every tick if holding click
void ABlobmentsV1PlayerController::SetNewMoveDestination(const FVector DestLocation)
{
	APawn* const MyPawn = GetPawn();
	if (MyPawn)
	{
		IMainPlayerInterface* MainInterface = Cast<IMainPlayerInterface>(MyPawn);
		if (MainInterface)
		{
			MainInterface->Execute_HoldingClickAtLocation(MyPawn, DestLocation);
		}

	}
}

void ABlobmentsV1PlayerController::OnSetDestinationPressed()
{
	// set flag to keep updating destination until released
	bMoveToMouseCursor = true;
}

void ABlobmentsV1PlayerController::OnSetDestinationReleased()
{
	bMoveToMouseCursor = false;
	APawn* const MyPawn = GetPawn();
	if (MyPawn)
	{
		IMainPlayerInterface* MainInterface = Cast<IMainPlayerInterface>(MyPawn);
		if (MainInterface)
		{
			MainInterface->Execute_ActivateBob(MyPawn);
		}

	}
}

void ABlobmentsV1PlayerController::OnCancel()
{
	bMoveToMouseCursor = false;
	APawn* const MyPawn = GetPawn();
	if (MyPawn)
	{
		IMainPlayerInterface* MainInterface = Cast<IMainPlayerInterface>(MyPawn);
		if (MainInterface)
		{
			MainInterface->Execute_DeActivateBob(MyPawn);
		}

	}
}


/*
void ABlobmentsV1PlayerController::MoveToTouchLocation(const ETouchIndex::Type FingerIndex, const FVector Location)
{
FVector2D ScreenSpaceLocation(Location);

// Trace to see what is under the touch location
FHitResult HitResult;
GetHitResultAtScreenPosition(ScreenSpaceLocation, CurrentClickTraceChannel, true, HitResult);
if (HitResult.bBlockingHit)
{
// We hit something, move there
SetNewMoveDestination(HitResult.ImpactPoint);
}
}*/