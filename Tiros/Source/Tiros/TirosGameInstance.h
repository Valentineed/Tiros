// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Interfaces/OnlineSessionInterface.h"
#include "Engine/GameInstance.h"
#include "TirosGameInstance.generated.h"

USTRUCT(BlueprintType)
struct FServerInfo
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	FString ServerName;
	UPROPERTY(BlueprintReadOnly)
	int32 CurrentPlayer;
	UPROPERTY(BlueprintReadOnly)
	int32 MaxPlayer;
	UPROPERTY(BlueprintReadOnly)
	int32 ServerArrayIndex;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FServerDelegate, FServerInfo, ServerInfo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FServerSearchingDelegate, bool, SearchingServers);

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

	FName ClientSessionName;

	UPROPERTY(BlueprintAssignable)
	FServerDelegate ServerDelegate;

	UPROPERTY(BlueprintAssignable)
	FServerSearchingDelegate ServerSearchingDelegate;

	TSharedPtr<FOnlineSessionSearch> SessionSearch;

	IOnlineSessionPtr SessionInterface;

    UFUNCTION(BlueprintCallable)
	void	CreateServer(FString ServerName, FString HostName);

	UFUNCTION(BlueprintCallable)
	void	FindServers();

	UFUNCTION(BlueprintCallable)
	void	JoinServer(int32 ArrayIndex);
};
