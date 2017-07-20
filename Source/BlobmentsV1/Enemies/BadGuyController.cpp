// Fill out your copyright notice in the Description page of Project Settings.

#include "BlobmentsV1.h"
#include "BadGuyController.h"
#include "BadGuyMain.h"
#include "BlobmentsV1GameMode.h"
#include "Kismet/KismetMathLibrary.h"

void ABadGuyController::Tick(float DeltaTime)
{
	if (PawnAsBadGuy)
	{
		if (AActor* Target = PawnAsBadGuy->GetTarget())
		{
			//  Find Rotation
			FVector DirectionToTarget = (Target->GetActorLocation() - PawnAsBadGuy->GetActorLocation()).GetSafeNormal2D();
			float DotToTarget = FVector::DotProduct(DirectionToTarget, PawnAsBadGuy->GetActorForwardVector());
			float SidewaysDotToTarget = FVector::DotProduct(DirectionToTarget, PawnAsBadGuy->GetActorRightVector());
			float DeltaYawDesired = FMath::Atan2(SidewaysDotToTarget, DotToTarget);

			PawnAsBadGuy->SetRotationInput(DeltaYawDesired);

			if (PawnAsBadGuy->BadGuyAIShouldAttack())
			{
				PawnAsBadGuy->AddAttackInput();
			}
		}
		else
		{
			Target = UGameplayStatics::GetPlayerPawn(this, 0);
			PawnAsBadGuy->SetTarget(Target);
		}


		
	}
}

void ABadGuyController::Possess(APawn* InPawn)
{
	Super::Possess(InPawn);
	PawnAsBadGuy = Cast<ABadGuyMain>(GetPawn());
	//Setting Game Mode
	if (GetWorld())
	{
		if (ABlobmentsV1GameMode* GameModeRef = (ABlobmentsV1GameMode*)GetWorld()->GetAuthGameMode())
		{
			GameModeRef->AddActorToBeat(PawnAsBadGuy);
		}

	}

}

void ABadGuyController::UnPossess()
{
	Super::UnPossess();
	PawnAsBadGuy = nullptr;
}
