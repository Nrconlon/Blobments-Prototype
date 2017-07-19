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

	Health = 100;
	Damage = 20.0f;
	YawSpeed = 90.0f;
	JumpDistance = 20.0f;
	JumpFrequency = 45.0f;

	//Attacking
	AttackDistance = 100.0f;
	AttackAngle = 30.0f;
	AttackCooldown = 2.0f;
	TimeBeforeAttack = 0.7f;

	//Movement
	TimeBeforeJump = 0.7f;
	GlideTime = 0.7f;
	GlideDistancePerSecond = 20.0f;
	IsRotating = false;
	IsGliding = false;


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

	if (Health >= 0)
	{
		DetermineMovement(DeltaTime);
	}

	ConsumeRotationInput();
	

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

void ABadGuyMain::BadGuyAI_Implementation(float DeltaSeconds)
{
	//bad guy always jumps
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

/*void ABadGuyMain::AttackInDirection_Implementation(FVector Direction)
{

}
*/


void ABadGuyMain::JumpForward_Implementation()
{
	FTimerHandle TempHandle;
	IsRotating = false;
	IsGliding = true;
	GetWorldTimerManager().SetTimer(TempHandle, this, &ABadGuyMain::Land, GlideTime, false, 0.0f);
}

void ABadGuyMain::Land_Implementation()
{
	IsGliding = false;
}

void ABadGuyMain::Activate()
{
	FTimerHandle TempHandle;
	IsRotating = true;
	GetWorldTimerManager().SetTimer(TempHandle, this, &ABadGuyMain::JumpForward, TimeBeforeAttack, false, 0.0f);
}

void ABadGuyMain::AddAttackInput()
{
	FTimerHandle UnusedHandle;
	bAttackInput = true;
	GetWorldTimerManager().SetTimer(UnusedHandle, this, &ABadGuyMain::ConsumeAttackInput, TimeBeforeAttack, false, 0.0f);

}

void ABadGuyMain::AddRotationInput(float DeltaYawDegrees)
{
	YawInput += DeltaYawDegrees;
}

float ABadGuyMain::GetRotationInput()
{
	return YawInput;
}

float ABadGuyMain::ConsumeRotationInput()
{
	float RetVal = YawInput;
	YawInput = 0.0f;
	return RetVal;
}

bool ABadGuyMain::GetAttackInput()
{
	return bAttackInput;
}

void ABadGuyMain::ConsumeAttackInput()
{

	if (BadGuyAIShouldAttack())
	{
		if (IDamageInterface* DamageTarget = Cast<IDamageInterface>(GetTarget()))
		{
			DamageTarget->ReceiveDamage(Damage);
		}

	}
	bAttackInput = false;
}

bool ABadGuyMain::TargetInRange()
{
	return false;
}

void ABadGuyMain::DetermineMovement(float DeltaSeconds)
{
	if (IsRotating)
	{
		float MaxYawThisFrame = YawSpeed * DeltaSeconds;
		FRotator DesiredRot = GetActorRotation() + FRotator(0.0f, FMath::Clamp(GetRotationInput(), -MaxYawThisFrame, MaxYawThisFrame), 0.0f);
		SetActorRotation(DesiredRot.Quaternion());
	}
	else if (IsGliding)
	{
		FVector DesiredMovement = GlideDistancePerSecond * DeltaSeconds * GetActorForwardVector();
		FVector OriginalLocation = GetActorLocation();
		FVector DesiredLoc = OriginalLocation + DesiredMovement;
		SetActorLocation(DesiredLoc, true);
	}
}


