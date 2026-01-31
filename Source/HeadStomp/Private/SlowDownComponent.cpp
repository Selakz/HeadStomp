// Fill out your copyright notice in the Description page of Project Settings.


#include "SlowDownComponent.h"
#include "GameFramework/Character.h"
#include "Damage/SlowDownDamageType.h"
#include "GameFramework/CharacterMovementComponent.h"


// Sets default values for this component's properties
USlowDownComponent::USlowDownComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}


// Called when the game starts
void USlowDownComponent::BeginPlay()
{
	Super::BeginPlay();

	if (ACharacter* Character = Cast<ACharacter>(GetOwner()))
	{
		Character->OnTakeAnyDamage.AddDynamic(this, &USlowDownComponent::OnTakeDamage);
		DefaultWalkSpeed = Character->GetCharacterMovement()->MaxWalkSpeed;
	}
}

void USlowDownComponent::OnTakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (!DamageType->IsA(USlowDownDamageType::StaticClass())) return;

	HandleSlowDownDamage(Damage);
}

void USlowDownComponent::HandleSlowDownDamage_Implementation(float Damage)
{
	if (ACharacter* Character = Cast<ACharacter>(GetOwner()))
	{
		Character->GetCharacterMovement()->MaxWalkSpeed = DefaultWalkSpeed * Damage;
		OnBeginSlowDown.Broadcast();
		GetWorld()->GetTimerManager().ClearTimer(SlowDownTimerHandle);
		GetWorld()->GetTimerManager().SetTimer(SlowDownTimerHandle, [this, Character]
		{
			Character->GetCharacterMovement()->MaxWalkSpeed = DefaultWalkSpeed;
			OnEndSlowDown.Broadcast();
		}, SlowDownDuration, false);
	}
}
