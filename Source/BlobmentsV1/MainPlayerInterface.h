// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MainPlayerInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UMainPlayerInterface : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

/**
 * 
 */
class BLOBMENTSV1_API IMainPlayerInterface
{
	GENERATED_IINTERFACE_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "MyCategory")
		bool HoldingClickAtLocation(const FVector DestLocation);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "MyCategory")
		bool ActivateBob();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "MyCategory")
		bool DeActivateBob();
	
	
};
