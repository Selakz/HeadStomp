// Fill out your copyright notice in the Description page of Project Settings.


#include "HSPlayerState.h"
#include "Net/UnrealNetwork.h"

void AHSPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AHSPlayerState, SelectedJob);
}
