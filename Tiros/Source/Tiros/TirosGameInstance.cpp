// Fill out your copyright notice in the Description page of Project Settings.


#include "TirosGameInstance.h"

#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Kismet/GameplayStatics.h"

UTirosGameInstance::UTirosGameInstance()
{
    ClientSessionName = FName("TestSession");
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
        GetWorld()->ServerTravel("/Game/Maps/Dev/LVL_Axel.LVL_Axel");
    }
}

void UTirosGameInstance::OnFindSessionComplete(const bool Succeeded)
{
    ServerSearchingDelegate.Broadcast(false);

    UE_LOG(LogTemp, Warning, TEXT("OnFindSessionComplete succeeded: %d"), Succeeded);

    if(Succeeded)
    {
        UE_LOG(LogTemp, Warning, TEXT("Found %d sessions"), SessionSearch->SearchResults.Num());

        int32 ArrayIndex = 0;
        for(auto &Result : SessionSearch->SearchResults)
        {
            if(Result.IsValid())
            {
                FServerInfo ServerInfo;
                FString ServerName = "EmptyName";
                FString HostName = "EmptyName";

                Result.Session.SessionSettings.Get(FName("SERVER_NAME"), ServerName);
                Result.Session.SessionSettings.Get(FName("HOST_NAME"), HostName);

                ServerInfo.ServerName = ServerName;
                ServerInfo.MaxPlayer = Result.Session.SessionSettings.NumPublicConnections;
                ServerInfo.CurrentPlayer = ServerInfo.MaxPlayer - Result.Session.NumOpenPublicConnections;
                ServerInfo.ServerArrayIndex = ArrayIndex;

                ServerDelegate.Broadcast(ServerInfo);
            }
            ArrayIndex++;
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

void UTirosGameInstance::CreateServer(FString ServerName, FString HostName)
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
    SessionSettings.Set(FName("SERVER_NAME"), ServerName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
    SessionSettings.Set(FName("HOST_NAME"), HostName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

    SessionInterface->CreateSession(0, ClientSessionName, SessionSettings);
}

void UTirosGameInstance::FindServers()
{
    ServerSearchingDelegate.Broadcast(true);

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

void UTirosGameInstance::JoinServer(const int32 ArrayIndex)
{
    const auto SearchResult = SessionSearch->SearchResults[ArrayIndex];

    if(SearchResult.IsValid())
    {
        UE_LOG(LogTemp, Warning, TEXT("Joining selected session at index %d"), ArrayIndex);
        SessionInterface->JoinSession(0, ClientSessionName, SearchResult);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to join session at index %d"), ArrayIndex);
        
    }
}
