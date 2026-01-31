// Copyright Epic Games, Inc. All Rights Reserved.


#include "HeadStompPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "InputMappingContext.h"
#include "HeadStompCameraManager.h"

AHeadStompPlayerController::AHeadStompPlayerController()
{
	// set the player camera manager class
	PlayerCameraManagerClass = AHeadStompCameraManager::StaticClass();
}

void AHeadStompPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// Add Input Mapping Context
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		for (UInputMappingContext* CurrentContext : DefaultMappingContexts)
		{
			Subsystem->AddMappingContext(CurrentContext, 0);
		}
	}
}
