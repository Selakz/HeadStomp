// Fill out your copyright notice in the Description page of Project Settings.


#include "HSBaseGameMode.h"
#include "Kismet/GameplayStatics.h"

UClass* AHSBaseGameMode::GetDefaultPawnClassForController_Implementation(AController* InController)
{
	if (const auto PlayerState = Cast<AHSPlayerState>(InController->PlayerState))
	{
		const auto Class = *CharacterMapping.Find(PlayerState->SelectedJob);
		return Class;
	}

	return nullptr;
}

APlayerController* AHSBaseGameMode::Login(
	UPlayer* NewPlayer, ENetRole InRemoteRole, const FString& Portal, const FString& Options, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	const auto Controller = Super::Login(NewPlayer, InRemoteRole, Portal, Options, UniqueId, ErrorMessage);
	if (const auto PlayerState = Cast<AHSPlayerState>(Controller->PlayerState))
	{
		const auto JobName = UGameplayStatics::ParseOption(Options, JobOptionKey);
		PlayerState->SelectedJob = *JobMapping.Find(JobName);
	}

	return Controller;
}
