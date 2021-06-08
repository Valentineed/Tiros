// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Interfaces/OnlineSessionInterface.h"
#include "Engine/GameInstance.h"
#include "TirosGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class TIROS_API UTirosGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UTirosGameInstance();

protected:
	virtual void Init() override;

	virtual void OnCreateSessionComplete(FName ServerName, bool Succeeded);

	virtual void OnFindSessionComplete(bool Succeeded);

	virtual void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

	TSharedPtr<FOnlineSessionSearch> SessionSearch;

	IOnlineSessionPtr SessionInterface;

    UFUNCTION(BlueprintCallable)
	void	CreateServer();

	UFUNCTION(BlueprintCallable)
	void	JoinServer();
};
