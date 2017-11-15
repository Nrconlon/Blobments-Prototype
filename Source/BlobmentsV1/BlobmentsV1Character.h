// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "MainPlayerInterface.h"
#include "DamageInterface.h"
#include "GameFramework/Character.h"
#include "BlobmentsV1Character.generated.h"

UCLASS(Blueprintable)
class ABlobmentsV1Character : public ACharacter, public IMainPlayerInterface, public IDamageInterface
{
	GENERATED_BODY()



public:
	ABlobmentsV1Character();
	UFUNCTION(BlueprintNativeEvent, Category = "MyCategory")
		void OnBobDeath();

	//Called every tick once past powered point
	UFUNCTION(BlueprintImplementableEvent, Category = "Abilities")
		void TogglePoweredMode(bool IsPowered);

	UFUNCTION(BlueprintImplementableEvent, Category = "MyCategory")
		void TakeDamageAnimation();

	UFUNCTION(BlueprintCallable, Category = "MyCategory")
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintNativeEvent, Category = "Abilities")
		void Explode();

	//MainPlayerInterface
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "MyCategory")
		bool HoldingClickAtLocation(const FVector DestLocation);
	virtual bool HoldingClickAtLocation_Implementation(const FVector DestLocation) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "MyCategory")
		bool ActivateBob();
	virtual bool ActivateBob_Implementation() override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "MyCategory")
		bool DeActivateBob();
	virtual bool DeActivateBob_Implementation() override;

	//IDamageInterface
	virtual void ReceiveDamage(int32 IncomingDamage) override;
	virtual int32 GetHealthRemaining() override { return Health; }

	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;

	//
	virtual void OnMovementModeChanged(EMovementMode PrevMovementMode,uint8 PreviousCustomMode) override;

	/** Returns TopDownCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns GetPotentialLandingDecal subobject **/
	FORCEINLINE class UDecalComponent* GetPotentialLandingDecal() { return PotentialLandingDecal; }
	/** Returns CurrentLandingDecal subobject **/
	FORCEINLINE class UDecalComponent* GetCurrentLandingDecal() { return CurrentLandingDecal; }

	virtual void BobPrepareJump(float DeltaSeconds);
	virtual void BobActivateJump();
	virtual void SetDecalLocations();
	virtual void SetCharacterRotation();

	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
		FVector CurrentLandPoint;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
		FVector AimingPoint;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
		FVector PotentialLandPoint;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
		FVector PotentialMomentum;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		FVector CDefaultStartMomentum;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		FVector CIncreaseMomentum;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		FVector CMaxMomentum;

	//Percentage of the maximum potential launch before being powered.  this will be 2 seconds times this number.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		float PercentBeforePowered;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		float CBreakingAlpha;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
		bool WindingUp;
	

private:
	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** A decal that projects to the cursor location. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Decal", meta = (AllowPrivateAccess = "true"))
	class UDecalComponent* PotentialLandingDecal;

	/** A decal that projects to the cursor location. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Decal", meta = (AllowPrivateAccess = "true"))
	class UDecalComponent* CurrentLandingDecal;

	/** A static mesh used to the art. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* MyMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Reference", meta = (AllowPrivateAccess = "true"))
	class ABlobmentsV1GameMode* GameModeRef;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
	class UPaperSpriteComponent* BobSprite;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
	class UArrowComponent* BobMainDirection;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats", meta = (AllowPrivateAccess = "true"))
		bool IsDead;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats", meta = (AllowPrivateAccess = "true"))
		int32 Health;

	/** Turn on when setting sprite to red, and off when setting to green in blueprints */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats", meta = (AllowPrivateAccess = "true"))
		bool IsRed;

	bool IsPowered;
	bool HasLanded;
	
	float MaxMomentumBeforePowered;
	

	

	/* Handle to manage the timer */
	FTimerHandle WindingUpTimerHandle;

	



};

