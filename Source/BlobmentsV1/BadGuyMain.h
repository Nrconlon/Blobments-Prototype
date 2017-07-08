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

	UFUNCTION(BlueprintNativeEvent, Category = "AI")
		bool BadGuyAIShouldAttack();
	virtual bool BadGuyAIShouldAttack_Implementation();

	UFUNCTION(BlueprintNativeEvent, Category = "AI")
		void AttackInDirection(FVector Direction);
	virtual void AttackInDirection_Implementation(FVector Direction);

	
	UFUNCTION(BlueprintNativeEvent, Category = "AI")
		void JumpInDirection(FVector Direction);
	virtual void JumpInDirection_Implementation(FVector Direction);


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
	float Damage;

	//* Sight distance no no target is present */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BadGuy", meta = (ClampMin = "0.0"))
	float SightDistance;

	//* Sight half angle when no target is present.  */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BadGuy", meta = (ClampMin = "0.0"))
	float SightAngle;

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

private:
	//* Min time between attacks */
	UPROPERTY(VisibleInstanceOnly, Category = "AI")
	AActor* TargetActor;

	//* Min time between attacks */
	UPROPERTY(VisibleInstanceOnly, Category = "AI")
	ABlobmentsV1Character* TargetBob;
	
};
