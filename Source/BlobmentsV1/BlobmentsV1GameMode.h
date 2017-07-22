// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "BadGuyMain.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/PlayerStart.h"
#include "BlobmentsV1GameMode.generated.h"

class ABadGuyMain;

UCLASS(minimalapi)
class ABlobmentsV1GameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ABlobmentsV1GameMode();

	virtual void BeginPlay() override;

	virtual void SetLandingDecalLocation(FVector location);
	virtual void SetLandingDecalVisibility(bool visible);
	void AddActorToBeat(AActor* IncomingActor);

	void StartTheBeat();
	void BadGuyMainBeat();
	void SpawnABadGuy();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement)
	class ACurrentLandingDecal* LandingDecal;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement)
	TArray<AActor*> AllActors;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement)
		TArray<ABadGuyMain*> BadGuyMains;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drum")
		float MainTimerBeat;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drum")
		TSubclassOf<class ABadGuyMain> BadGuyBlueprint;

	//UFUNCTION(BlueprintImplementableEvent, meta = (FriendlyName = "SpawnBadGuysFromBP"))
		//virtual void SpawnBadGuyFromBP(FVector Location, FVector Rotation);

private:
	FTimerHandle DrumTimeHandle;
	TArray<APlayerStart*> AllSpawnPoints;


	

};



