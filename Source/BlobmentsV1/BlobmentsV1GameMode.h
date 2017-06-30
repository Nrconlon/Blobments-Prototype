// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/GameModeBase.h"
#include "BlobmentsV1GameMode.generated.h"

UCLASS(minimalapi)
class ABlobmentsV1GameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ABlobmentsV1GameMode();

	virtual void BeginPlay() override;

	virtual void SetLandingDecalLocation(FVector location);
	virtual void SetLandingDecalVisibility(bool visible);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement)
	class ACurrentLandingDecal* LandingDecal;
};



