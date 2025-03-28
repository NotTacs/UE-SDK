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

SDK::UEngine* SDK::UEngine::GetEngine()
{
	if (!SDK::UE::Core::GEngine)
	{
		for (SDK::FUObjectItem* Object : *SDK::UE::Core::GObjects)
		{
			if (!Object) continue;
			if (!Object->Object) continue;
			if (Object->Object->GetName().ToString().contains("FortEngine_"))
			{
				SDK::UE::Core::GEngine = (SDK::UEngine*)Object->Object;
			}
		}
	}

	return SDK::UE::Core::GEngine;
}

SDK::UClass* SDK::UKismetStringLibrary::StaticClass()
{
	static UClass* Class = nullptr;
	if (!Class)
		Class = reinterpret_cast<UClass*>(SDK::UE::Core::GObjects->FindObjectFast("KismetStringLibrary"));
	return Class;
}

SDK::FName SDK::UKismetStringLibrary::Conv_StringToName(const FString& InString)
{
	static UFunction* Func = nullptr;
	if (!Func)
		Func = StaticClass()->FindFunctionByName("Conv_StringToName");

	struct KismetStringLibrary_Conv_StringToName
	{
		FString InString;
		FName ReturnValue;
	};

	KismetStringLibrary_Conv_StringToName Params;

	Params.InString = std::move(InString);

	StaticClass()->GetDefaultObj()->ProcessEvent(Func, &Params);

	return Params.ReturnValue;
}