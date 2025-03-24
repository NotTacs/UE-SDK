#include "../SDK.h"
#include "Engine.h"

SDK::UGameViewportClient* SDK::UEngine::GameViewport()
{
	return GET_PROPERTY_VALUE<UGameViewportClient*>( this, "GameViewport");
}

SDK::UWorld* SDK::UGameViewportClient::World()
{
	return GET_PROPERTY_VALUE<UWorld*>(this, "World");
}

SDK::AFortGameStateAthena* SDK::UWorld::GameState()
{
	return GET_PROPERTY_VALUE<AFortGameStateAthena*>(this, "GameState");
}