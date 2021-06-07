// Copyright Epic Games, Inc. All Rights Reserved.

#include "TirosGameMode.h"
#include "TirosHUD.h"
#include "TirosCharacter.h"
#include "UObject/ConstructorHelpers.h"

ATirosGameMode::ATirosGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = ATirosHUD::StaticClass();
}
