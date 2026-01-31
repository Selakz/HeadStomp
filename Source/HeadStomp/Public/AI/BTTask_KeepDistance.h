// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_KeepDistance.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class HEADSTOMP_API UBTTask_KeepDistance : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_KeepDistance();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	virtual void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult) override;

private:
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector TargetActorKey;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector DistanceToTargetKey;

	UPROPERTY(EditAnywhere, Category = "Config")
	float DesiredDistance = 400.f;

	UPROPERTY(EditAnywhere, Category = "Config")
	float DestinationNoise = 150.f;

	UPROPERTY(EditAnywhere, Category = "Config")
	float OrbitAngle = 45.f;

	UPROPERTY(EditAnywhere, Category = "Config")
	float RotationNoise = 15.f;

	UPROPERTY(EditAnywhere, Category = "Config")
	float TaskDuration = 3.0f;

	float RemainingTime;
};
