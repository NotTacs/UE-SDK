#include "../SDK.h"
#include "Fortnite.h"

SDK::UClass* SDK::FPlaylistPropertyArray::GetScriptClass()
{
	static SDK::UClass* Class = nullptr;

	if (!Class)
		Class = reinterpret_cast<UClass*>(SDK::UE::Core::GObjects->FindObjectFast("PlaylistPropertyArray"));
	return Class;
}

SDK::UObject* SDK::FPlaylistPropertyArray::BasePlaylist()
{
	return *reinterpret_cast<UObject**>(__int64(this) + GET_PROPERTYSTRUCT_OFFSET(this, "BasePlaylist"));
}

SDK::UClass* SDK::UIExtensionClass::GetScriptClass()
{
	static SDK::UClass* Class = nullptr;
	if (!Class)
		Class = reinterpret_cast<UClass*>(SDK::UE::Core::GObjects->FindObjectFast("PlaylistUIExtension")); //add the other one later
	return Class;
}