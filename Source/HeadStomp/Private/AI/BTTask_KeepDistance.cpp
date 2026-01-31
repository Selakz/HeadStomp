// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_KeepDistance.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Navigation/PathFollowingComponent.h"

UBTTask_KeepDistance::UBTTask_KeepDistance()
{
	NodeName = "Keep Distance";
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_KeepDistance::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	const UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();

	if (!AIController || !BB) return EBTNodeResult::Failed;

	AActor* Target = Cast<AActor>(BB->GetValueAsObject(TargetActorKey.SelectedKeyName));
	if (!Target) return EBTNodeResult::Failed;

	AIController->SetFocus(Target);

	// Get the rotating angle
	const FVector PlayerLocation = Target->GetActorLocation();
	const FVector NPCLocation = AIController->GetPawn()->GetActorLocation();
	const FVector DirectionFromPlayer = (NPCLocation - PlayerLocation).GetSafeNormal2D();
	const float RandomDirection = FMath::RandBool() ? 1.f : -1.f;
	const float GoalAngle = (OrbitAngle + FMath::RandRange(-RotationNoise, RotationNoise)) * RandomDirection;
	// Get the target position
	const FVector MoveDirection = DirectionFromPlayer.RotateAngleAxis(GoalAngle, FVector::UpVector);
	FVector GoalLocation = PlayerLocation + MoveDirection * DesiredDistance;
	FVector LocationNoise = FMath::VRand() * DestinationNoise;
	LocationNoise.Z = 0;
	GoalLocation += LocationNoise;

	// Project the location for AI to move safely
	UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
	FNavLocation ProjectedLocation;
	if (NavSys && NavSys->ProjectPointToNavigation(GoalLocation, ProjectedLocation))
	{
		GoalLocation = ProjectedLocation.Location;
	}

	AIController->MoveToLocation(GoalLocation, 50.f);

	RemainingTime = TaskDuration;
	return EBTNodeResult::InProgress;
}

void UBTTask_KeepDistance::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	RemainingTime -= DeltaSeconds;

	const AAIController* AIController = OwnerComp.GetAIOwner();
	if (RemainingTime <= 0.f || (AIController && AIController->GetMoveStatus() == EPathFollowingStatus::Type::Idle))
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}

void UBTTask_KeepDistance::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	if (AAIController* AIController = OwnerComp.GetAIOwner()) AIController->ClearFocus(EAIFocusPriority::Gameplay);
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);
}
