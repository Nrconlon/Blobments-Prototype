// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "BlobmentsV1.h"
#include "BlobmentsV1GameMode.h"
#include "BadGuyMain.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"
#include "BlobmentsV1PlayerController.h"
#include "CurrentLandingDecal.h"
#include "BlobmentsV1Character.h"
#include <vector>

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

	static ConstructorHelpers::FObjectFinder<UBlueprint> ItemBlueprint(TEXT("Blueprint'/Game/TopDownCPP/Blueprints/MainBadGuyBP'"));
	if (ItemBlueprint.Object) {
		BadGuyBlueprint = (UClass*)ItemBlueprint.Object->GetClass();
	}

	MainTimerBeat = 2.1f;
	UWorld* const World = GetWorld();
	if (World)
	{
		for (TObjectIterator<APlayerStart> Itr; Itr; ++Itr)
		{
			if (Itr->GetWorld() == World)
			{
				AllSpawnPoints.Add(*Itr);
			}
		}
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
	StartTheBeat();
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

void ABlobmentsV1GameMode::StartTheBeat()
{
	GetWorld()->GetTimerManager().SetTimer(DrumTimeHandle, this, &ABlobmentsV1GameMode::BadGuyMainBeat,MainTimerBeat,true, false);
}

void ABlobmentsV1GameMode::BadGuyMainBeat()
{
	TArray<ABadGuyMain*> badGuysToRemove;
	for (ABadGuyMain* mainBadGuy : BadGuyMains)
	{
		if (mainBadGuy)
		{
			mainBadGuy->Activate();
		}
		else
		{
			badGuysToRemove.Add(mainBadGuy);
		}
	}

	for(ABadGuyMain* mainBadGuy : badGuysToRemove)
	{
		RemoveActorFromBeat(mainBadGuy);
	}
	SpawnABadGuy();
}

void ABlobmentsV1GameMode::SpawnABadGuy()
{
	if (AllSpawnPoints.Num() > 0)
	{
		int32 RandomNumber = FMath::RandRange(0, AllSpawnPoints.Num() - 1);
		FVector Location = AllSpawnPoints[RandomNumber]->GetActorLocation();
		FRotator Rotation = AllSpawnPoints[RandomNumber]->GetActorRotation();
		FActorSpawnParameters SpawnInfo;
		if (UWorld* World = GetWorld())
		{
			FActorSpawnParameters SpawnParams;
			//UGameplayStatics::BeginSpawningActorFromBlueprint(GetWorld(), BadGuyBlueprint,)
			World->SpawnActor<ABadGuyMain>(BadGuyBlueprint, Location, Rotation, SpawnParams);
		}
	}
}


void ABlobmentsV1GameMode::AddActorToBeat(AActor* IncomingActor)
{
	AllActors.Add(IncomingActor);
	if(ABadGuyMain* ActorAsBadGuyMain = Cast<ABadGuyMain>(IncomingActor))
	{
		BadGuyMains.Add(ActorAsBadGuyMain);
	}
}

void ABlobmentsV1GameMode::RemoveActorFromBeat(AActor* IncomingActor)
{
	AllActors.Remove(IncomingActor);
	if (ABadGuyMain* ActorAsBadGuyMain = Cast<ABadGuyMain>(IncomingActor))
	{
		BadGuyMains.Remove(ActorAsBadGuyMain);
	}
}

