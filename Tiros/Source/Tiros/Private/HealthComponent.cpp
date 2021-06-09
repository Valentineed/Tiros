// Fill out your copyright notice in the Description page of Project Settings.


#include "Tiros/Public/HealthComponent.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void UHealthComponent::RestoreHealth(const float HealthRestore)
{
    Health += HealthRestore;
	if(Health > MaxHealth)
	{
		Health = MaxHealth;
	}
}

void UHealthComponent::RemoveHealth(const float HealthRemove)
{
    Health -= HealthRemove;
	if(Health < 0.f)
	{
		Health = 0.f;
	}
}

