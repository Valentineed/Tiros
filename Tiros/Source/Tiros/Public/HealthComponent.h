// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

#define DEFAULT_HEALTH 100.f

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TIROS_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();

    /**
	 * Restore a certain amount of Health. Health can not exceed MaxHealth.
	 * @param HealthRestore Health amount to restore.
	 */
    void	RestoreHealth(const float HealthRestore);

    /**
     * Remove a certain amount of Health. Health can not get below 0.
     * @param HealthRemove Health amount to remove.
     */
    void	RemoveHealth(const float HealthRemove);

    /**
	 * Reset current Health to MaxHealth value.
	 */
	void	ResetHealth() { Health = MaxHealth; }

    /**
	 * Set current Health.
	 * @param NewHealth New current Health.
	 */
	void	SetHealth(const float NewHealth) { Health = NewHealth; }

    /**
	 * Set maximum possible Health.
	 * @param NewMaxHealth New Maximum possible Health.
	 */
	void	SetMaxHealth(const float NewMaxHealth) { MaxHealth = NewMaxHealth; }

    /**
	 * Return current Health.
	 * @return Current Health.
	 */
	[[nodiscard]] float	GetHealth() const { return Health; }

	/**
	 * Return max Health.
	 * @return Max Health.
	 */
	[[nodiscard]] float	GetMaxHealth() const { return MaxHealth; }

protected:

    /**
	 * Current Health.
	 */
	UPROPERTY(EditAnywhere)
	float Health = DEFAULT_HEALTH;

    /**
	 * Maximum possible Health.
	 */
	UPROPERTY(EditAnywhere)
	float MaxHealth = DEFAULT_HEALTH;
};
