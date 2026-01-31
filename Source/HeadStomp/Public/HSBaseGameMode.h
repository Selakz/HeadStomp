// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HSPlayerState.h"
#include "GameFramework/GameMode.h"
#include "HSBaseGameMode.generated.h"

/**
 * 
 */
UCLASS()
class HEADSTOMP_API AHSBaseGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString JobOptionKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FString, EJob> JobMapping;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<EJob, UClass*> CharacterMapping;

	virtual UClass* GetDefaultPawnClassForController_Implementation(AController* InController) override;

	virtual APlayerController* Login(
		UPlayer* NewPlayer, ENetRole InRemoteRole, const FString& Portal, const FString& Options, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;
};
