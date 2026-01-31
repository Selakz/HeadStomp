// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SlowDownComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSlowDownStateChangeEvent);

UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class HEADSTOMP_API USlowDownComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	USlowDownComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SlowDownDuration = 1.0f;

	UPROPERTY(BlueprintAssignable, Category="Event")
	FOnSlowDownStateChangeEvent OnBeginSlowDown;

	UPROPERTY(BlueprintAssignable, Category="Event")
	FOnSlowDownStateChangeEvent OnEndSlowDown;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnTakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);

	UFUNCTION(NetMulticast, Reliable)
	void HandleSlowDownDamage(float Damage);

private:
	float DefaultWalkSpeed = 1;

	FTimerHandle SlowDownTimerHandle;
};
