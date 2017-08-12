// Fill out your copyright notice in the Description page of Project Settings.

#include "BlobmentsV1.h"
#include "BadGuyMain.h"
#include "BlobmentsV1Character.h"
#include "Kismet/KismetMathLibrary.h"
#include "Global_Log.h"

DEFINE_LOG_CATEGORY_STATIC(NeilsLog, All, All)


// Sets default values
ABadGuyMain::ABadGuyMain()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	Health = 100;
	Damage = 20.0f;
	JumpDistance = 20.0f;
	JumpFrequency = 45.0f;
	PushPower = 70.0f;
	PushHeight = 245.f;

	//Attacking
	AttackDistance = 100.0f;
	AttackAngle = 30.0f;
	AttackCooldown = 2.0f;
	TimeBeforeAttack = 0.7f;

	//Movement
	TimeBeforeJump = 1.05f;
	GlideTime = 0.5f;
	GlideDistancePerSecond = 200.0f;
	IsRotating = false;
	IsGliding = false;

	YawSpeed = 70.0f / TimeBeforeJump;
	YawInput = 0.0f;

	IsAlive = 1;


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

	if (Health >= 0 && IsAlive)
	{
		DetermineMovement(DeltaTime);
	}
	

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

void ABadGuyMain::BadGuyDeath_Implementation()
{
	FVector OldLocation = GetActorLocation();
	SetActorLocation(FVector(OldLocation.X, OldLocation.Y, OldLocation.Z - 50), true);
}

void ABadGuyMain::ReceiveDamage(int32 IncomingDamage)
{
}

int32 ABadGuyMain::GetHealthRemaining()
{
	if (IsAlive)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

FVector ABadGuyMain::Bump(AActor* Bumper, FVector Velocity, bool IsPowerUp)
{
	FVector ReturnVelocity = Velocity;
	if (IsPowerUp)
	{
		IsAlive = 0;
		BadGuyDeath();
	}
	else
	{
		if (IDamageInterface* DamageTarget = Cast<IDamageInterface>(Bumper))
		{
			DamageTarget->ReceiveDamage(Damage);
			ReturnVelocity = PushPower * UKismetMathLibrary::GetDirectionUnitVector(GetActorLocation(),Bumper->GetActorLocation());
			ReturnVelocity = FVector(ReturnVelocity.X, ReturnVelocity.Y, PushHeight);
		}
	}
	return ReturnVelocity;
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
	GetWorldTimerManager().SetTimer(TempHandle, this, &ABadGuyMain::Land, GlideTime, false, GlideTime);
}

void ABadGuyMain::Land_Implementation()
{
	IsGliding = false;
}

void ABadGuyMain::Activate()
{
	FTimerHandle TempHandle;
	IsRotating = true;
	GetWorldTimerManager().SetTimer(TempHandle, this, &ABadGuyMain::JumpForward, TimeBeforeJump, false, TimeBeforeJump);
}

void ABadGuyMain::AddAttackInput()
{
	FTimerHandle UnusedHandle;
	bAttackInput = true;
	GetWorldTimerManager().SetTimer(UnusedHandle, this, &ABadGuyMain::ConsumeAttackInput, TimeBeforeAttack, false, TimeBeforeAttack);

}

void ABadGuyMain::SetRotationInput(float DeltaYawDegrees)
{
	YawInput = DeltaYawDegrees;
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
			//This form of attacking turned off, replaced with *Bump* on touch from player
			//DamageTarget->ReceiveDamage(Damage);  
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
		//Yaw Input goes from 0 to 3.1
		float CurrentChangeInAngle = GetRotationInput();
		float MaxYawThisFrame = YawSpeed * DeltaSeconds;
		FRotator DesiredRot;
		if (CurrentChangeInAngle > 0.4f || CurrentChangeInAngle < -0.4f)
		{
			if (CurrentChangeInAngle < 0)
			{
				MaxYawThisFrame = MaxYawThisFrame * -1.0f;
			}
			DesiredRot = GetActorRotation() + FRotator(0.0f, MaxYawThisFrame, 0.0f);
		}
		else 
		{
			DesiredRot = GetActorRotation() + FRotator(0.0f, MaxYawThisFrame  * (CurrentChangeInAngle / 0.4f), 0.0f);
		}
		
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


