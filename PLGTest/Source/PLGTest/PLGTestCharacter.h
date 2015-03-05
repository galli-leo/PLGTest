// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once

#include "PLGTestCharacter.generated.h"

UCLASS(Blueprintable)
class APLGTestCharacter : public ACharacter
{
	GENERATED_UCLASS_BODY()

	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	TSubobjectPtr<UCameraComponent> TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	TSubobjectPtr<class USpringArmComponent> CameraBoom;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		TSubobjectPtr<class UStaticMeshComponent> CameraMesh;

};

