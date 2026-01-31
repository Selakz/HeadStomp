// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "HeadStompGameInstance.generated.h"

UENUM(BlueprintType)
enum class EGameType : uint8
{
	None,
	Pve,
	Pvp
};

UENUM(BlueprintType)
enum class ESkill : uint8
{
	None,
	SlowDownBullet,
	LessHighJumpCooldown
};

/**
 * The main instance containing lifetime info of the game
 */
UCLASS()
class HEADSTOMP_API UHeadStompGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Setting")
	EGameType SelectedMode = EGameType::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Setting")
	ESkill SelectedSkill = ESkill::None;
};
