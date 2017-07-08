// Fill out your copyright notice in the Description page of Project Settings.

#include "BlobmentsV1.h"
#include "BadGuyMain.h"
#include "BlobmentsV1Character.h"


// Sets default values
ABadGuyMain::ABadGuyMain()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	Health = 100.0f;
	Damage = 20.0f;
	SightDistance = 500.0f;
	SightAngle = 60.0f;
	YawSpeed = 90.0f;
	JumpDistance = 20.0f;
	JumpFrequency = 45.0f;
	AttackDistance = 100.0f;
	AttackAngle = 30.0f;
	AttackCooldown = 2.0f;


}

// Called when the game starts or when spawned
void ABadGuyMain::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABadGuyMain::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABadGuyMain::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ABadGuyMain::SetTarget(AActor* NewTarget)
{
	TargetActor = NewTarget;
	TargetBob = Cast<ABlobmentsV1Character>(NewTarget);
}

AActor* ABadGuyMain::GetTarget()
{
	return TargetActor;
}

ABlobmentsV1Character* ABadGuyMain::GetTargetAsBob()
{
	return TargetBob;
}

bool ABadGuyMain::BadGuyAIShouldAttack_Implementation()
{
	if (AActor* Target = GetTarget())
	{
		//Attack our Target if we're in range
		FVector OurLocation = GetActorLocation();
		FVector DirectionToTarget = (Target->GetActorLocation() - OurLocation.GetSafeNormal2D());
		float DotToTarget = FVector::DotProduct(DirectionToTarget, GetActorForwardVector());
		if (DotToTarget >= FMath::Cos(FMath::DegreesToRadians(AttackAngle)))
		{
			float DistSqXY = FVector::DistSquaredXY(Target->GetActorLocation(), OurLocation);
			if (DistSqXY <= (AttackDistance * AttackDistance))
			{
				//Cooldown checked in attack command.  
				return true;
			}
		}

	}
	return false;
}

void ABadGuyMain::AttackInDirection_Implementation(FVector Direction)
{

}


void ABadGuyMain::JumpInDirection_Implementation(FVector Direction)
{
	float DotToTarget = FVector::DotProduct(Direction, GetActorForwardVector());
	float SidewaysDotToTarget = FVector::DotProduct(Direction, GetActorRightVector());
	float DeltaYawDesired = FMath::Atan2(SidewaysDotToTarget, DotToTarget);
}
