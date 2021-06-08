// Fill out your copyright notice in the Description page of Project Settings.


#include "TirosGameInstance.h"

#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Kismet/GameplayStatics.h"

UTirosGameInstance::UTirosGameInstance()
{
}

void UTirosGameInstance::Init()
{
    if(IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get())
    {
        SessionInterface = Subsystem->GetSessionInterface();
        if(SessionInterface.IsValid())
        {
            // Bind delegates here

            SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UTirosGameInstance::OnCreateSessionComplete);
            SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UTirosGameInstance::OnFindSessionComplete);
            SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UTirosGameInstance::OnJoinSessionComplete);
        }
    }
}

void UTirosGameInstance::OnCreateSessionComplete(FName ServerName, const bool Succeeded)
{
    UE_LOG(LogTemp, Warning, TEXT("OnCreateSessionComplete succeeded: %d"), Succeeded);

    if(Succeeded)
    {
        GetWorld()->ServerTravel("/Game/FirstPersonCPP/Maps/FirstPersonExampleMap.FirstPersonExampleMap");
    }
}

void UTirosGameInstance::OnFindSessionComplete(const bool Succeeded)
{
    UE_LOG(LogTemp, Warning, TEXT("OnFindSessionComplete succeeded: %d"), Succeeded);

    if(Succeeded)
    {
        auto SearchResults = SessionSearch->SearchResults;

        UE_LOG(LogTemp, Warning, TEXT("Found %d sessions"), SearchResults.Num());

        if(SearchResults.Num())
        {
            SessionInterface->JoinSession(0, "TestSession", SearchResults[0]);
        }
        
    }
}

void UTirosGameInstance::OnJoinSessionComplete(const FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
    UE_LOG(LogTemp, Warning, TEXT("OnJoinSessionComplete"));

    if(APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0))
    {
        FString JoinAddress;
        if(SessionInterface->GetResolvedConnectString(SessionName, JoinAddress))
        {
            PlayerController->ClientTravel(JoinAddress, TRAVEL_Absolute);
        }
    }
}

void UTirosGameInstance::CreateServer()
{
    UE_LOG(LogTemp, Warning, TEXT("Creating server"));

    FOnlineSessionSettings SessionSettings;
    SessionSettings.bAllowJoinInProgress = true;
    SessionSettings.bIsDedicated = true;
    if(IOnlineSubsystem::Get()->GetSubsystemName() != "NULL")
    {
        SessionSettings.bIsLANMatch = false;
    }
    else
    {
        SessionSettings.bIsLANMatch = true;
    }
    SessionSettings.bShouldAdvertise = true;
    SessionSettings.bUsesPresence = true;
    SessionSettings.NumPublicConnections = 10;

    SessionInterface->CreateSession(0, FName("TestSession"), SessionSettings);
}

void UTirosGameInstance::JoinServer()
{
    UE_LOG(LogTemp, Warning, TEXT("Joining server"));

    SessionSearch = MakeShareable(new FOnlineSessionSearch);
    SessionSearch->bIsLanQuery = false;

    if (IOnlineSubsystem::Get()->GetSubsystemName() != "NULL")
    {
        SessionSearch->bIsLanQuery = false;
    }
    else
    {
        SessionSearch->bIsLanQuery = true;
    }
    SessionSearch->MaxSearchResults = 10000;
    SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

    SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
}
