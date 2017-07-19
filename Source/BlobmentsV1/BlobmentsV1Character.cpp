// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "BlobmentsV1.h"
#include "BlobmentsV1Character.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Components/DecalComponent.h"
#include "Kismet/HeadMountedDisplayFunctionLibrary.h"
#include "PaperSpriteComponent.h"
#include "Components/ArrowComponent.h"
#include "BlobmentsV1GameMode.h"
#include "Global_Log.h"
#include "CurrentLandingDecal.h"

DEFINE_LOG_CATEGORY_STATIC(NeilsLog, All, All)

ABlobmentsV1Character::ABlobmentsV1Character()
{

	//Stats
	IsDead = false;
	Health = 100;

	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(60.f, 60.0f);

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = false; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 10000.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	//Setting Game Mode
	if (GetWorld()) 
	{
		GameModeRef = (ABlobmentsV1GameMode*)GetWorld()->GetAuthGameMode();
	}
		
	
	// Create static Mesh
/*	not needed, replaced with sprite
MyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticMeshAsset(TEXT("StaticMesh'/Game/MobileStarterContent/Props/MaterialSphere.MaterialSphere'"));
	if (StaticMeshAsset.Succeeded())
	{
		MyMesh->SetStaticMesh(StaticMeshAsset.Object);
	}
	static ConstructorHelpers::FObjectFinder<UMaterial> MaterialAsset(TEXT("Material'/Game/MobileStarterContent/Props/Materials/M_MaterialSphere_Plain.M_MaterialSphere_Plain'"));
	if (MaterialAsset.Succeeded())
	{
		MyMesh->SetMaterial(0,MaterialAsset.Object);
	}
	MyMesh->SetRelativeScale3D(FVector(1.1f, 1.1f, 1.1f));
	MyMesh->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
	*/ 
	//Create sprite
	if (!RootComponent)
	{
		RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("BobBase"));
	}

	BobMainDirection = CreateDefaultSubobject<UArrowComponent>(TEXT("BobMainDirection"));
	BobMainDirection->SetupAttachment(RootComponent);

	BobSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("BobSprite"));
	BobSprite->SetupAttachment(BobMainDirection);


	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->bAbsoluteRotation = true; // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 1500.f;
	CameraBoom->CameraLagSpeed = 2.0f;
	CameraBoom->bUsePawnControlRotation = false;
	CameraBoom->bEnableCameraLag = true;
	CameraBoom->bEnableCameraRotationLag = false;
	CameraBoom->RelativeRotation = FRotator(-90.f, 0.f, 0.f);
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

										  // Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	//TopDownCameraComponent->ProjectionMode = ECameraProjectionMode::Orthographic;
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
	//TopDownCameraComponent->OrthoWidth = 4024.0f;
	//TopDownCameraComponent->AspectRatio = 3.0f / 4.0f;

	// Create a decal in the world to show the character's potential landing location
	PotentialLandingDecal = CreateDefaultSubobject<UDecalComponent>("PotentialLandingDecal");
	PotentialLandingDecal->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UMaterial> DecalMaterialAssetPotential(TEXT("Material'/Game/TopDownCPP/Blueprints/M_PotentialLandingDecal'"));//M_Cursor_Decal.M_PotentialLandingDecal
	if (DecalMaterialAssetPotential.Succeeded())
	{
		PotentialLandingDecal->SetDecalMaterial(DecalMaterialAssetPotential.Object);
	}
	PotentialLandingDecal->DecalSize = FVector(16.0f, 32.0f, 600.0f);
	PotentialLandingDecal->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f).Quaternion());
	PotentialLandingDecal->FadeScreenSize = 0.0f;
	PotentialLandingDecal->SetVisibility(false); 

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	//Movement components
	CurrentLandPoint = FVector(0.f, 0.f, 0.f);
	PotentialLandPoint = FVector(0.f, 0.f, 0.f);
	AimingPoint = FVector(0.f, 0.f, 0.f);
	PotentialMomentum = FVector(0.f, 0.f, 0.f);
	WindingUp = false;
	CDefaultStartMomentum = FVector(200.f, 200.f, 245.f);
	//increase per second
	//max time in air 1 sec, min time in air 0.5 sec, max wind up time 2 sec.  
	CIncreaseMomentum = FVector(1000.f, 1000.f, 120.25f);
	CMaxMomentum = CDefaultStartMomentum + (CIncreaseMomentum * 2);

	//breaking friction constant
	GetCharacterMovement()->BrakingFrictionFactor = 0.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 100.f;
	CBreakingAlpha = 0.6f;

}

void ABlobmentsV1Character::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

	//if (PotentialLandingDecal != nullptr)
	//{
	//	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	//	{
	//		FHitResult TraceHitResult;
	//		PC->GetHitResultUnderCursor(ECC_Visibility, true, TraceHitResult);
	//		FVector CursorFV = TraceHitResult.ImpactNormal;
	//		FRotator CursorR = CursorFV.Rotation();
	//		
	//	}
	//}

	//Breaking friction percentage on top of the constant breaking friction
	//Amount per second
	if (GetCharacterMovement()->IsMovingOnGround())
	{
		FVector amountToDecrease = GetVelocity() / CBreakingAlpha;  //Greater than 2 seconds.
		/*FVector direction = GetVelocity().GetSafeNormal();
		direction = direction * -1;
		*/
		amountToDecrease = amountToDecrease * -1 * DeltaSeconds;
		LaunchCharacter(amountToDecrease, false, false);

	}
	if (WindingUp)
	{
		BobPrepareJump(DeltaSeconds);
	}

	SetDecalLocations();
	if (!GetCharacterMovement()->IsFalling())
	{
		SetCharacterRotation();
	}



}

//This is called every tick from controller
bool ABlobmentsV1Character::HoldingClickAtLocation_Implementation(const FVector DestLocation)
{
	AimingPoint = DestLocation;
	if (!WindingUp && GetCharacterMovement()->IsMovingOnGround())
	{
		WindingUp = true;
		PotentialMomentum = CDefaultStartMomentum;
	}
	
	return true;
}

bool ABlobmentsV1Character::ActivateBob_Implementation()
{
	CurrentLandPoint = AimingPoint;
	WindingUp = false;
	if (!GetCharacterMovement()->IsFalling())
	{
		BobActivateJump();
	}
	return true;
}

bool ABlobmentsV1Character::DeActivateBob_Implementation()
{
	WindingUp = false;
	GetWorld()->GetTimerManager().ClearTimer(WindingUpTimerHandle);
	PotentialMomentum = FVector(0.f, 0.f, 0.f);
	return true;
}

void ABlobmentsV1Character::ReceiveDamage(int32 IncomingDamage)
{
	if (Health >= 0)
	{
		Health = Health - IncomingDamage;
	}
	if (Health <= 0 && !IsDead)
	{
		OnBobDeath();
	}
}

//Movement Functions
void ABlobmentsV1Character::BobPrepareJump(float DeltaSeconds)
{
	if (CMaxMomentum.Size() > PotentialMomentum.Size())
	{
		PotentialMomentum = PotentialMomentum + (CIncreaseMomentum * DeltaSeconds);
	}

}
//only called if not falling, launch character based on wind up
void ABlobmentsV1Character::BobActivateJump()
{
	 
	FVector direction = AimingPoint - GetActorLocation();
	direction.Normalize();
	direction = FVector(direction.X, direction.Y, 1.f);
	PotentialMomentum = PotentialMomentum * direction;
	LaunchCharacter(PotentialMomentum, false, false);

	//Turn on decal, turned back off once landed.
	CurrentLandPoint = PotentialLandPoint;
	GameModeRef->SetLandingDecalLocation(CurrentLandPoint);
	GameModeRef->SetLandingDecalVisibility(true);
	FRotator Rotation = FRotator(0.0f, 90.0f, 0.0f);
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	//ACurrentLandingDecal* MCurrentLandingDecal = GetWorld()->SpawnActor<ACurrentLandingDecal>(ACurrentLandingDecal::StaticClass(), CurrentLandPoint, Rotation, SpawnInfo);
	//MCurrentLandingDecal->SetLifeSpan(PotentialMomentum.Z / GetCharacterMovement()->GetGravityZ() * 2);//set life span to landing time.
	//static ConstructorHelpers::FObjectFinder<UMaterial> DecalMaterialAssetCurrent(TEXT("Material'/Game/TopDownCPP/Blueprints/M_CurrentLandingDecal'"));
	//if (DecalMaterialAssetCurrent.Succeeded())
	//{
	//	MCurrentLandingDecal->Decal->SetDecalMaterial(DecalMaterialAssetCurrent.Object);
	//}
	//MCurrentLandingDecal->Decal->DecalSize = FVector(16.0f, 32.0f, 600.0f);
	//MCurrentLandingDecal->Decal->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f).Quaternion());
	//MCurrentLandingDecal->Decal->FadeScreenSize = 0.0f;
	/*CurrentLandingDecal->SetWorldLocation(CurrentLandPoint);
	CurrentLandingDecal->SetVisibility(true);*/

	PotentialMomentum = FVector(0.f, 0.f, 0.f);
}

void ABlobmentsV1Character::SetDecalLocations()
{
	if(WindingUp)
	{
		//Locations will be mesed if character jumps in the air.(not possible at this time)
		float groundLocationZ = 50.f;//Set to 50 for now

		FVector direction = AimingPoint - GetActorLocation();
		direction.Normalize();
		direction = FVector(direction.X, direction.Y, 1.f);
		FVector directedMomentum = PotentialMomentum * direction; //Rotate momentum in correct direction

		FVector ActualPotentialMomentum = directedMomentum + GetVelocity(); //Take into account current velocity(both of which are directed now)
		float timeToLand = ActualPotentialMomentum.Z / GetCharacterMovement()->GetGravityZ() * 2;
		PotentialLandPoint = ActualPotentialMomentum * timeToLand;  //Time in air
		PotentialLandPoint = FVector(GetActorLocation().X + ActualPotentialMomentum.X, GetActorLocation().Y + ActualPotentialMomentum.Y , groundLocationZ);

		PotentialLandingDecal->SetWorldLocation(PotentialLandPoint);
		//Uncomment for potential landing decal
		//PotentialLandingDecal->SetVisibility(true);  
	}

}

void ABlobmentsV1Character::OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode)
{
	if (GetCharacterMovement()->IsFalling())
	{
		//GetCharacterMovement()->bOrientRotationToMovement = true;
	}
	else
	{
		GameModeRef->SetLandingDecalVisibility(false);
		GetCharacterMovement()->bOrientRotationToMovement = false;
	}

}

void ABlobmentsV1Character::SetCharacterRotation()
{
	FVector direction = AimingPoint - GetActorLocation();
	FVector directionNoZ = FVector(direction.X, direction.Y, 0.f);
	FHitResult RotationSweepHitResult;
	K2_SetActorRelativeRotation(directionNoZ.Rotation(), true, RotationSweepHitResult, false);
}

void ABlobmentsV1Character::OnBobDeath()
{
	IsDead = true;
}

