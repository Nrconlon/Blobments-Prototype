// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "BadGuyMain.generated.h"


class ABlobmentsV1Character;

UCLASS()
class BLOBMENTSV1_API ABadGuyMain : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABadGuyMain();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable, Category = "AI")
		void SetTarget(AActor* NewTarget);
	

	UFUNCTION(BlueprintCallable, Category = "AI")
		AActor* GetTarget();

	UFUNCTION(BlueprintCallable, Category = "AI")
		ABlobmentsV1Character* GetTargetAsBob();

	//called on tick
	UFUNCTION(BlueprintNativeEvent, Category = "AI")
		void BadGuyAI(float DeltaSeconds);
	virtual void BadGuyAI_Implementation(float DeltaSeconds);

	//Animating walking
	UFUNCTION(BlueprintImplementableEvent, Category = "AI")
		void BadGuyJump();

	UFUNCTION(BlueprintImplementableEvent, Category = "AI")
		void BadGuyAttack();

	UFUNCTION(BlueprintNativeEvent, Category = "AI")
		bool BadGuyAIShouldAttack();
	virtual bool BadGuyAIShouldAttack_Implementation();

	UFUNCTION(BlueprintCallable, Category = "Pawn|Input", meta = (Keywords = "AddInput"))
	virtual void AddAttackInput();

		//Rotation stuff
		/** Add rotation equal to the yaw (in degrees) provided. */
		UFUNCTION(BlueprintCallable, Category = "Pawn|Input", meta = (Keywords = "AddInput"))
			virtual void AddRotationInput(float DeltaYawDegrees);

		/** Get rotation input. Returns pre-clear value. */
		UFUNCTION(BlueprintCallable, Category = "Pawn|Input", meta = (Keywords = "ConsumeInput"))
			virtual float GetRotationInput();

		/** Get (and clear) rotation input. Returns pre-clear value. */
		UFUNCTION(BlueprintCallable, Category = "Pawn|Input", meta = (Keywords = "ConsumeInput"))
			virtual float ConsumeRotationInput();
	

	UFUNCTION(BlueprintCallable, Category = "Pawn|Input", meta = (Keywords = "ConsumeInput"))
		virtual bool GetAttackInput();

	UFUNCTION(BlueprintCallable, Category = "Pawn|Input", meta = (Keywords = "ConsumeInput"))
		virtual void ConsumeAttackInput();

	UFUNCTION(BlueprintCallable, Category = "Pawn|Input", meta = (Keywords = "ConsumeInput"))
		virtual bool TargetInRange();

	UFUNCTION(BlueprintCallable, Category = "Pawn|Input", meta = (Keywords = "AddInput"))
		virtual void DetermineMovement(float DeltaSeconds);

	UFUNCTION(BlueprintNativeEvent, Category = "AI")
		void JumpForward();
	virtual void JumpForward_Implementation();

	UFUNCTION(BlueprintNativeEvent, Category = "AI")
		void Land();
	virtual void Land_Implementation();

	UFUNCTION(BlueprintCallable, Category = "Pawn|Input", meta = (Keywords = "ConsumeInput"))
		virtual void Activate();




protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//* Camera effect, if any, to play when player is hit by badguy.  */
	UPROPERTY(EditAnywhere)
	TSubclassOf<UCameraShake> HitShake;

	//* Current health */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BadGuy", meta = (ClampMin = "0.0"))
	float Health;

	//* Bad guy damage per hit*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BadGuy", meta = (ClampMin = "0.0"))
	int32 Damage;

	//* How fast bad guy turns in degrees per second */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BadGuy", meta = (ClampMin = "0.0"))
	float YawSpeed;

	//* How far bad guy jumps  */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BadGuy", meta = (ClampMin = "0.0"))
	float JumpDistance;

	//* How often bad guy jumps  */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BadGuy", meta = (ClampMin = "0.0"))
	float JumpFrequency;

	//* Attack distance  */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BadGuy", meta = (ClampMin = "0.0"))
	float AttackDistance;

	//* Half-angle (in degrees) for attack  */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BadGuy", meta = (ClampMin = "0.0"))
	float AttackAngle;

	//* Min time between attacks */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BadGuy", meta = (ClampMin = "0.0"))
	float AttackCooldown;

	//* Min time between attacks */
	UPROPERTY(Transient, VisibleInstanceOnly,BlueprintReadWrite, Category = "BadGuy")
	float AttackAvailableTime;

	//* How long it takes to attack */
	UPROPERTY(Transient, VisibleInstanceOnly, BlueprintReadWrite, Category = "BadGuy")
		float TimeBeforeAttack;

	UPROPERTY(Transient, VisibleInstanceOnly, BlueprintReadWrite, Category = "BadGuy")
		float TimeBeforeJump;

	//* Glide time */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BadGuy")
		float GlideTime;

	//* Glide distance */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BadGuy")
		float GlideDistancePerSecond;


private:
	//* Min time between attacks */
	UPROPERTY(VisibleInstanceOnly, Category = "AI")
	AActor* TargetActor;

	//* Min time between attacks */
	UPROPERTY(VisibleInstanceOnly, Category = "AI")
	ABlobmentsV1Character* TargetBob;

	// Current attack button status.
	uint32 bAttackInput : 1;

	// Current rotation input.
	float YawInput;

	bool IsRotating;
	bool IsGliding;
	
};
