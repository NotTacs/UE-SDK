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
		inline int UFunction__Func = -1;
		inline uintptr_t ObjObjectsAddr = 0;
		inline uintptr_t FName__ToString = 0;
		inline uintptr_t FName__ToStringVoid = 0;
		inline uintptr_t UObject__ProcessEvent = 0;
		namespace MemberOffsets
		{
			inline int UStruct__SuperStruct = -1;
			inline int UStruct__Children = -1;
			inline int UStruct__MinAllignment = -1;
			inline int UStruct__Script = -1;
			inline int UStruct__PropertyLink = -1;
			inline int UStruct__RefLink = -1;
			inline int UStruct__DestructorLink = -1;
			inline int UStruct__PostConstructLink = -1;
			inline int UStruct__ScriptObjectReferences = -1;
			inline int UProperty__Offset_Internal = -1;
			/*inline int UClass__ClassDefaultObject = -1;*/
		}
	}
	namespace GameInfo
	{
		uintptr_t GetBaseAddress();

		inline size_t GetMemorySize() {
			MEMORY_BASIC_INFORMATION mbi;
			uintptr_t baseAddress = GetBaseAddress();

			if (VirtualQuery(reinterpret_cast<LPCVOID>(baseAddress), &mbi, sizeof(mbi)) == 0) {
				return 0;
			}

			return mbi.RegionSize;
		}
	}

	namespace UE
	{
		inline std::string EngineVersion = "";

		float GetFortniteVersion();

		double GetEngineVersion();

		int GetFortniteCL();
	}

	namespace UE::Memory
	{
		bool IsOffsetValid(uintptr_t Address);
	}

	namespace UE::Core
	{
		inline std::unique_ptr<SDK::FUObjectArray> GObjects;
	}
	bool Init();
}