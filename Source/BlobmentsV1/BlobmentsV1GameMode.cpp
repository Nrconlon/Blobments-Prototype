// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "BlobmentsV1.h"
#include "BlobmentsV1GameMode.h"
#include "BlobmentsV1PlayerController.h"
#include "CurrentLandingDecal.h"
#include "BlobmentsV1Character.h"

ABlobmentsV1GameMode::ABlobmentsV1GameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = ABlobmentsV1PlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDownCPP/Blueprints/TopDownCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

void ABlobmentsV1GameMode::BeginPlay()
{
	Super::BeginPlay();
	UWorld* const World = GetWorld();
	if (World)
	{
		LandingDecal = (ACurrentLandingDecal*) World->SpawnActor<ACurrentLandingDecal>(ACurrentLandingDecal::StaticClass());
	}
}

void ABlobmentsV1GameMode::SetLandingDecalLocation(FVector Location)
{
	if (LandingDecal)
	{
		LandingDecal->SetActorLocation(Location);
	}
}

void ABlobmentsV1GameMode::SetLandingDecalVisibility(bool visibility)
{
	if (LandingDecal)
	{
		LandingDecal->SetActorHiddenInGame(!visibility);
	}

}