// Fill out your copyright notice in the Description page of Project Settings.

#include "BlobmentsV1.h"
#include "DamageInterface.h"


// This function does not need to be modified.
UDamageInterface::UDamageInterface(const class FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
}

// Add default functionality here for any IDamageInterface functions that are not pure virtual.

FVector IDamageInterface::Bump(AActor* Bumper, FVector Velocity, bool IsPowerUp)
{
	return Velocity;
}
