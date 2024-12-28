// Copyright Epic Games, Inc. All Rights Reserved.

#include "Json_ProjectGameMode.h"
#include "Json_ProjectCharacter.h"
#include "UObject/ConstructorHelpers.h"

AJson_ProjectGameMode::AJson_ProjectGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
