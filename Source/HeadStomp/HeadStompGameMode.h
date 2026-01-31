// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "HeadStompGameMode.generated.h"

/**
 *  Simple GameMode for a first person game
 */
UCLASS(abstract)
class AHeadStompGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AHeadStompGameMode();

	UFUNCTION(BlueprintNativeEvent)
	UClass* GetDefaultPawnClassForController_Implementation(AController* InController) override;
};
