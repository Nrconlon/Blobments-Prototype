// Fill out your copyright notice in the Description page of Project Settings.

#include "BlobmentsV1.h"
#include "BadGuyController.h"
#include "BadGuyMain.h"

void ABadGuyController::Tick(float DeltaTime)
{
	if (PawnAsBadGuy)
	{
		FVector DirectionToTarget = PawnAsBadGuy->GetActorForwardVector();
		if (AActor* Target = PawnAsBadGuy->GetTarget())
		{
			//We have a target.
			DirectionToTarget = (Target->GetActorLocation() - PawnAsBadGuy->GetActorLocation().GetSafeNormal2D());
			float DotToTarget = FVector::DotProduct(DirectionToTarget, PawnAsBadGuy->GetActorForwardVector());
			float SidewaysDotToTarget = FVector::DotProduct(DirectionToTarget, PawnAsBadGuy->GetActorRightVector());
			float DeltaYawDesired = FMath::Atan2(SidewaysDotToTarget, DotToTarget);
		}

		if (PawnAsBadGuy->BadGuyAIShouldAttack())
		{
			PawnAsBadGuy->AddAttackInput();
		}
		else
		{
			//PawnAsBadGuy->JumpInDirection(DirectionToTarget);
		}
	}
}

void ABadGuyController::Possess(APawn* InPawn)
{
	Super::Possess(InPawn);
	PawnAsBadGuy = Cast<ABadGuyMain>(GetPawn());
}

void ABadGuyController::UnPossess()
{
	Super::UnPossess();
	PawnAsBadGuy = nullptr;
}
