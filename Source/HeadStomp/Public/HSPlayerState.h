// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "HSPlayerState.generated.h"

UENUM(BlueprintType)
enum class EJob : uint8
{
	None,
	Warrior,
	IcyShooter,
	Dancer
};

/**
 * 
 */
UCLASS()
class HEADSTOMP_API AHSPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Game Setting")
	EJob SelectedJob = EJob::None;

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
