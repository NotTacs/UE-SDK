#pragma once
#include <windows.h>
#include <iostream>
#include <thread>
#include <string>
#include <cstring>
#include <memory>
#include <type_traits>
#include <fstream>
#include <sstream>
#include <mutex>
#include <iomanip>
#include <vector>
#include <unordered_map>
#include "../memcury.h"
#include "SDK/Basic.h"
#include "SDK/Engine.h"
#include "SDK/Fortnite.h"

namespace SDK
{
	namespace Addresses
	{

		inline uintptr_t ObjObjectsAddr = 0;
		inline uintptr_t FName__ToString = 0;
		inline uintptr_t FName__ToStringVoid = 0;
		inline uintptr_t UObject__ProcessEvent = 0;
		namespace MemberOffsets
		{
			inline int UStruct__SuperStruct = -1;
			inline int UStruct_Children = -1;
			inline int UStruct_MinAllignment = -1;
			inline int UStruct_Script = -1;
			inline int UStruct_PropertyLink = -1;
			inline int UStruct_RefLink = -1;
			inline int UStruct_DestructorLink = -1;
			inline int UStruct_PostConstructLink = -1;
			inline int UStruct_ScriptObjectReferences = -1;
			inline int UProperty__Offset_Internal = -1;
			inline int UFunction__Func = -1;
			/*inline int UClass__ClassDefaultObject = -1;*/
		}
	}
	namespace GameInfo
	{
		uintptr_t GetBaseAddress();
	}

	namespace UE
	{
		inline std::string EngineVersion = "";

		float GetFortniteVersion();

		double GetEngineVersion();

		int GetFortniteCL();
	}

	namespace UE::Core
	{
		inline std::unique_ptr<SDK::FUObjectArray> GObjects;
		inline SDK::UEngine* GEngine;
		inline SDK::UWorld* GWorld;
	}

	bool InitGObjects();
	bool InitMemberOffsets();
	bool InitFName();
	bool SetupEngineVersion();
	bool InitProcessEvent();
	bool Init();
}