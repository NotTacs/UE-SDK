#include "SDK.h"

bool SDK::Init()
{
	Memcury::Scanner Scanner = Memcury::Scanner::FindPattern("FF 95 ? ? ? ? 48 8B 6C 24");

	if (Scanner.Get() == 0)
	{
		Scanner = Memcury::Scanner::FindPattern("FF 97 ? ? ? ? 48 8B 6C 24");

		if (Scanner.Get() == 0)
		{
			Scanner = Memcury::Scanner::FindPattern("FF 95 ? ? ? ? 48 8B 6C 24", true);
		}
	}

	SDK::Addresses::UFunction__Func = *Scanner.AbsoluteOffset(2).GetAs<int*>();

	std::cout << "UFunction::Func Offset: " << SDK::Addresses::UFunction__Func << std::endl;

	Scanner = Memcury::Scanner::FindPattern("48 8B 05 ? ? ? ? 48 8B 0C C8 48 8B 04 D1");

	bool bChunked = true;

	if (Scanner.Get() == 0)
	{
		bChunked = false;

		Scanner = Memcury::Scanner::FindPattern("48 8B 05 ? ? ? ? 48 8D 14 C8 EB 03 49 8B D6 8B 42 08 C1 E8 1D A8 01 0F 85 ? ? ? ? F7 86 ? ? ? ? ? ? ? ?");

		if (Scanner.Get() == 0)
		{
			Scanner = Memcury::Scanner::FindPattern("48 8B 05 ? ? ? ? 48 8D 1C C8 81 4B ? ? ? ? ? 49 63 76 30");
		}
	}

	if (Scanner.Get() != 0)
		SDK::Addresses::ObjObjectsAddr = Scanner.RelativeOffset(3).Get();

	if (SDK::Addresses::ObjObjectsAddr == 0)
	{
		std::cout << "Failed to find ObjObjectsAddress" << std::endl;
		return false;
	}

	std::cout << SDK::Addresses::ObjObjectsAddr - SDK::GameInfo::GetBaseAddress() << std::endl;

	try
	{
		SDK::UE::Core::GObjects = std::make_unique<FUObjectArray>(SDK::Addresses::ObjObjectsAddr, bChunked);
	}
	catch (const std::exception& e)
	{
		std::cout << "Unexpected Exception" << std::endl;
	}

	if (!SDK::UE::Core::GObjects)
	{
		std::cout << "failed to initalize GObjects" << std::endl;
		return false;
	}

	Scanner = Memcury::Scanner::FindPattern("E8 ? ? ? ? 83 7C 24 ? ? 48 8D 3D ? ? ? ? 48 8B EF 48 8D 8E");

	if (Scanner.Get() == 0)
	{
		Scanner = Memcury::Scanner::FindPattern("E8 ? ? ? ? 83 78 ? ? 0F 84 ? ? ? ? 4C 8B 30", true);
	}

	if (Scanner.Get() != 0)
		SDK::Addresses::FName__ToString = Scanner.RelativeOffset(1).Get();

	

	if (SDK::Addresses::FName__ToString == 0)
	{
		std::cout << "Failed to initalize FString& FName::ToString(FName* thisPtr, FString&)" << std::endl;
		return false;
	}

	Scanner = Memcury::Scanner(SDK::Addresses::FName__ToString).ScanFor({ 0xE8 });

	if (Scanner.Get() != 0)
		SDK::Addresses::FName__ToStringVoid = Scanner.RelativeOffset(1).Get();

	if (SDK::Addresses::FName__ToStringVoid == 0)
	{
		std::cout << "Failed to initalize void FName::ToString(FName* thisPtr, FString&)" << std::endl;
		return false;
	}

	

	UFunction* GetEngineVersionFN = reinterpret_cast<UFunction*>(SDK::UE::Core::GObjects->FindObjectFast("GetEngineVersion"));

	std::cout << uintptr_t(GetEngineVersionFN->Func()) - SDK::GameInfo::GetBaseAddress() << std::endl;

	Scanner = Memcury::Scanner(uintptr_t(GetEngineVersionFN->Func())).ScanFor({ 0xE8 });

	if (Scanner.Get())
	{
		static FString& (*GetEngineVersion)(FString & retstr) = decltype(GetEngineVersion)(Scanner.RelativeOffset(1).Get());

		FString TempString = GetEngineVersion(TempString);
		SDK::UE::EngineVersion = TempString.ToString();
		std::cout << SDK::UE::EngineVersion << std::endl;
	}
	else
	{
		std::cout << "Failed to fetch Engine Version." << std::endl;
		return false;
	}

	std::cout << "Engine_Version: " << SDK::UE::GetEngineVersion() << std::endl;
	std::cout << "Fortnite_Version: " << SDK::UE::GetFortniteVersion() << std::endl;
	std::cout << "Fortnite_CL: " << SDK::UE::GetFortniteCL() << std::endl;

	Addresses::MemberOffsets::UStruct__SuperStruct = SDK::UE::GetEngineVersion() >= 4.22 ? 0x40 : 0x30;
	Addresses::MemberOffsets::UStruct__Children = Addresses::MemberOffsets::UStruct__SuperStruct + 0x8;
	Addresses::MemberOffsets::UStruct__MinAllignment = Addresses::MemberOffsets::UStruct__Children + 0xC;
	Addresses::MemberOffsets::UStruct__Script = Addresses::MemberOffsets::UStruct__Children + 0x10;
	Addresses::MemberOffsets::UStruct__PropertyLink = Addresses::MemberOffsets::UStruct__SuperStruct + 0x30;
	Addresses::MemberOffsets::UStruct__RefLink = Addresses::MemberOffsets::UStruct__PropertyLink + 0x8;
	Addresses::MemberOffsets::UStruct__DestructorLink = Addresses::MemberOffsets::UStruct__RefLink + 0x8;
	Addresses::MemberOffsets::UStruct__PostConstructLink = Addresses::MemberOffsets::UStruct__DestructorLink + 0x8;
	Addresses::MemberOffsets::UStruct__ScriptObjectReferences = Addresses::MemberOffsets::UStruct__PostConstructLink + 0x8;

	SDK::Addresses::MemberOffsets::UProperty__Offset_Internal = SDK::UE::GetFortniteVersion() >= 12.10 && std::floor(SDK::UE::GetFortniteVersion()) > 20 ? 0x4C : 0x44;

	Scanner = Memcury::Scanner::FindPattern("41 FF 92 ? ? ? ? F6 C3", true);

	if (Scanner.Get())
	{
		SDK::Addresses::UObject__ProcessEvent = Scanner.ScanFor({ 0x40, 0x55 },false).Get();

		std::cout << SDK::Addresses::UObject__ProcessEvent - SDK::GameInfo::GetBaseAddress() << std::endl;
	}

	UEngine* Engine = nullptr;
	UObject* KismetSystemLibrary = nullptr;

	for (FUObjectItem* Object : *SDK::UE::Core::GObjects)
	{
		if (!Object) continue;

		if (!Object->Object) continue;

		if (Object->Object->GetName().ToString().contains("FortEngine_"))
		{
			Engine = (UEngine*)Object->Object;
		}

		if (Object->Object->GetName() == "Default__KismetSystemLibrary")
		{
			KismetSystemLibrary = Object->Object;
		}
	}

	if (!Engine)
	{
		return false;
	}

	std::cout << Engine->GetName() << std::endl;

	auto GameViewportOffset = Engine->GetClass()->FindPropertyByName("GameViewport")->Offset_Internal();
	if (GameViewportOffset != 0)
	{
		auto& GameViewport = *reinterpret_cast<UObject**>(__int64(Engine) + GameViewportOffset);

		std::cout << GameViewport->GetName().ToString() << std::endl;

		auto WorldOffset = GameViewport->GetClass()->FindPropertyByName("World")->Offset_Internal();

		struct GameplayStatics_SpawnObject
		{
			UClass* ObjectClass;
			UObject* Outer;
			UObject* ReturnValue;
		};

		GameplayStatics_SpawnObject SpawnObjectParams;

		SDK::UObject* Object2 = SDK::UE::Core::GObjects->FindObjectFast("Default__GameplayStatics");

		SDK::UFunction* Func = Object2->GetClass()->FindFunctionByName("SpawnObject");

		if (Func)
		{
			std::cout << "Valid" << std::endl;
		}

		SDK::UClass* ObjectClass = reinterpret_cast<SDK::UClass*>(SDK::UE::Core::GObjects->FindObjectFast("FortConsole"));

		SpawnObjectParams.ObjectClass = ObjectClass;
		SpawnObjectParams.Outer = GameViewport;

		Object2->ProcessEvent(Func, &SpawnObjectParams);

		auto ViewportConsoleOffset = GameViewport->GetClass()->FindPropertyByName("ViewportConsole")->Offset_Internal();

		*(SDK::UObject**)(__int64(GameViewport) + ViewportConsoleOffset) = SpawnObjectParams.ReturnValue;
	}



	return true;
}

uintptr_t SDK::GameInfo::GetBaseAddress()
{
	return uintptr_t(GetModuleHandle(0));
}

float SDK::UE::GetFortniteVersion()
{
	size_t LastDot = EngineVersion.find('-', EngineVersion.find('-') + 1);
	std::string ParsedString = EngineVersion.substr(LastDot + 1);

	if (ParsedString.contains("Cert") || ParsedString.contains("Live"))
	{
		switch (SDK::UE::GetFortniteCL()) //prob wanna use cl for most of these builds
		{
		case 3541083:
			ParsedString = "1.2";
			break;
		case 3700114:
			ParsedString = "1.72";
			break;
		case 3724489:
			ParsedString = "1.8";
			break;
		case 3729133:
			ParsedString = "1.81";
			break;
		case 3741772:
			ParsedString = "1.82";
			break;
		case 3757339:
			ParsedString = "1.9";
			break;
		case 3775276:
			ParsedString = "1.91";
			break;
		case 3790078:
			ParsedString = "1.10";
			break;
		case 3807424:
			ParsedString = "1.11";
			break;
		}
	}

	return std::stof(ParsedString);
}

double SDK::UE::GetEngineVersion()
{
	size_t FirstDotPosition = EngineVersion.find('.');
	size_t SecondDotPostition = EngineVersion.find('.', FirstDotPosition + 1);
	std::string ParsedString = EngineVersion.substr(0, SecondDotPostition);

	return std::stod(ParsedString);
}

int SDK::UE::GetFortniteCL()
{
	size_t dashPos = EngineVersion.find('-');
	size_t plusPos = EngineVersion.find('+', dashPos);

	std::string ParsedString = "";

	if (dashPos != std::string::npos && plusPos != std::string::npos) {
		ParsedString = EngineVersion.substr(dashPos + 1, plusPos - dashPos - 1);
	}

	return std::stoi(ParsedString);
}

bool SDK::UE::Memory::IsOffsetValid(uintptr_t Address)
{
	uintptr_t baseAddress = SDK::GameInfo::GetBaseAddress();
	uintptr_t maxAddress = baseAddress + SDK::GameInfo::GetMemorySize();
	return (Address >= baseAddress && Address < maxAddress);
}