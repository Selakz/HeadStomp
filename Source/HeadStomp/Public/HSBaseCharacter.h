// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "HSBaseCharacter.generated.h"


DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStartCooldownEvent, float, CooldownEndTime);

UCLASS()
class HEADSTOMP_API AHSBaseCharacter : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="HeadStomp", meta = (AllowPrivateAccess = "true"))
	UBoxComponent* HeadHitBox;

	FTimerHandle JumpTimerHandle;
	float DefaultJumpVelocity;

protected:
	UPROPERTY(Replicated)
	bool bIsHighJumping = false;

	UPROPERTY(Replicated)
	bool bIsInJumpCooldown = false;

public:
	AHSBaseCharacter();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HeadStomp")
	float BasicStompDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HeadStomp")
	float JumpCooldown;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HeadStomp")
	float StompImpulseStrength = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HeadStomp")
	float HighJumpVelocity;

	UFUNCTION(BlueprintPure, Category = "HeadStomp")
	bool CanHighJump();

	UPROPERTY(BlueprintAssignable, Category = "HeadStomp")
	FOnStartCooldownEvent OnStartCooldown;

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void BeginPlay() override;

	/** Handles move inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoMove(float Right, float Forward);

	/** Handles jump start inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoJumpStart();

	UFUNCTION(Server, Reliable)
	virtual void Server_DoJumpStart();

	/** Handles jump end inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoJumpEnd();

	virtual void Landed(const FHitResult& Hit) override;

	UFUNCTION()
	virtual void OnHeadHitBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                                 const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnCapsuleOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                              const FHitResult& SweepResult);
};
