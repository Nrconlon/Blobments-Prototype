// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "DamageInterface.generated.h"

UENUM(BlueprintType)
enum class EDamageType : uint8
{
	Unknown,
	BobSmash,
	EnemySmash
};

UINTERFACE(MinimalAPI)
class UDamageInterface : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

/**
*
*/
class BLOBMENTSV1_API IDamageInterface
{
	GENERATED_IINTERFACE_BODY()

public:
	virtual void ReceiveDamage(int32 IncomingDamage) = 0;
	virtual int32 GetHealthRemaining() = 0;
};
