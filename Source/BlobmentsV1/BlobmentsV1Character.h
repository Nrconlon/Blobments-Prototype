// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "MainPlayerInterface.h"
#include "GameFramework/Character.h"
#include "BlobmentsV1Character.generated.h"

UCLASS(Blueprintable)
class ABlobmentsV1Character : public ACharacter, public IMainPlayerInterface
{
	GENERATED_BODY()

	

public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "MyCategory")
		bool HoldingClickAtLocation(const FVector DestLocation);
		virtual bool HoldingClickAtLocation_Implementation(const FVector DestLocation) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "MyCategory")
		bool ActivateBob();
		virtual bool ActivateBob_Implementation() override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "MyCategory")
		bool DeActivateBob();
		virtual bool DeActivateBob_Implementation() override;

	ABlobmentsV1Character();

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
	

	

	/* Handle to manage the timer */
	FTimerHandle WindingUpTimerHandle;
	



};

