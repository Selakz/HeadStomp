// Fill out your copyright notice in the Description page of Project Settings.


#include "HSBaseCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Damage/BasicHealthDamageType.h"
#include "Damage/StompImpulseDamageType.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AHSBaseCharacter

AHSBaseCharacter::AHSBaseCharacter()
{
	bReplicates = true;

	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
	GetCapsuleComponent()->SetCapsuleSize(34.0f, 96.0f);
	GetMesh()->SetOwnerNoSee(true);

	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;
	GetCharacterMovement()->AirControl = 0.5f;

	HeadHitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("HeadHitBox"));
	HeadHitBox->SetupAttachment(GetCapsuleComponent());
	HeadHitBox->SetBoxExtent(FVector(20.0f, 20.0f, 10.0f));
	HeadHitBox->SetRelativeLocation(FVector(0.0f, 0.0f, 100.0f));
	HeadHitBox->SetCollisionProfileName(TEXT("Trigger"));
}

bool AHSBaseCharacter::CanHighJump()
{
	return !bIsInJumpCooldown;
}

void AHSBaseCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AHSBaseCharacter, bIsHighJumping);
}

void AHSBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AHSBaseCharacter::OnCapsuleOverlap);
	HeadHitBox->OnComponentBeginOverlap.AddDynamic(this, &AHSBaseCharacter::OnHeadHitBoxOverlap);
	DefaultJumpVelocity = GetCharacterMovement()->JumpZVelocity;
}

void AHSBaseCharacter::DoMove(float Right, float Forward)
{
	AddMovementInput(GetActorRightVector(), Right);
	AddMovementInput(GetActorForwardVector(), Forward);
}

void AHSBaseCharacter::DoJumpStart()
{
	if (CanHighJump())
	{
		GetCharacterMovement()->JumpZVelocity = HighJumpVelocity;
		if (CanJump()) bIsHighJumping = true;
	}
	Jump();
	if (!HasAuthority()) Server_DoJumpStart();
}

void AHSBaseCharacter::Server_DoJumpStart_Implementation()
{
	if (CanHighJump())
	{
		GetCharacterMovement()->JumpZVelocity = HighJumpVelocity;
		if (CanJump()) bIsHighJumping = true;
	}
	Jump();
}

void AHSBaseCharacter::DoJumpEnd()
{
	StopJumping();
}

void AHSBaseCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

	GetCharacterMovement()->JumpZVelocity = DefaultJumpVelocity;
	if (const auto Actor = Hit.GetActor(); !(Actor && Actor->IsA(ACharacter::StaticClass())) && bIsHighJumping)
	{
		bIsHighJumping = false;
		bIsInJumpCooldown = true;
		OnStartCooldown.Broadcast(GetWorld()->GetTimeSeconds() + JumpCooldown);
		GetWorld()->GetTimerManager().ClearTimer(JumpTimerHandle);
		GetWorld()->GetTimerManager().SetTimer(JumpTimerHandle, [this]
		{
			bIsInJumpCooldown = false;
		}, JumpCooldown, false);
	}
}

void AHSBaseCharacter::OnHeadHitBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                           const FHitResult& SweepResult)
{
	constexpr float NormalRange = -0.7f;
	if (ACharacter* Stomper = Cast<ACharacter>(OtherActor); Stomper && Stomper != this)
	{
		const auto RelativeVelocity = Stomper->GetVelocity() - GetVelocity();
		if (RelativeVelocity.Z < 0.0f && SweepResult.ImpactNormal.Z < NormalRange)
		{
			UE_LOG(LogTemp, Warning, TEXT("%s was stomped by %s !"), *GetName(), *Stomper->GetName());
			UGameplayStatics::ApplyDamage(Stomper, StompImpulseStrength, GetController(), this, UStompImpulseDamageType::StaticClass());
		}
	}
}

void AHSBaseCharacter::OnCapsuleOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                        const FHitResult& SweepResult)
{
	constexpr float NormalRange = 0.7f;
	if (AHSBaseCharacter* Stomped = Cast<AHSBaseCharacter>(OtherActor); Stomped && Stomped != this)
	{
		const auto RelativeVelocity = GetVelocity() - Stomped->GetVelocity();
		if (RelativeVelocity.Z < 0.0f && SweepResult.ImpactNormal.Z > NormalRange && OtherComp == Stomped->HeadHitBox)
		{
			UE_LOG(LogTemp, Warning, TEXT("%s stomped %s !"), *GetName(), *Stomped->GetName());
			UGameplayStatics::ApplyDamage(Stomped, BasicStompDamage, GetController(), this, UBasicHealthDamageType::StaticClass());
		}
	}
}
