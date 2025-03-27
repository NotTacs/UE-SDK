#include "..\SDK.h"
#include "Basic.h"


SDK::UField* SDK::UField::Next()
{
	static int NextAddress = -1;

	if (NextAddress == -1)
	{
		NextAddress = 0x28;
	}

	return *reinterpret_cast<UField**>(__int64(this) + NextAddress);
}

SDK::FUObjectArray::FUObjectArray(uintptr_t ObjectAddress, bool bIsChunked)
	: ObjObjectsAddr(ObjectAddress), bUseChunkArray(bIsChunked)
{
	if (ObjectAddress == 0)
		throw std::runtime_error("FUObjectArray::FUObjectArray constructer recieved a null address");
}

SDK::FUObjectItem* SDK::FUObjectArray::GetObjectPtr(int32 Index)
{
	if (this->bUseChunkArray)
		return reinterpret_cast<FChunkedFixedUObjectArray*>(ObjObjectsAddr)->GetObjectPtr(Index);
	else
		return reinterpret_cast<FFixedUObjectArray*>(ObjObjectsAddr)->GetObjectPtr(Index);

	return nullptr;
}

SDK::FUObjectItem const* SDK::FUObjectArray::GetObjectPtr(int32 Index) const
{
	if (this->bUseChunkArray)
		return reinterpret_cast<FChunkedFixedUObjectArray*>(ObjObjectsAddr)->GetObjectPtr(Index);
	else
		return reinterpret_cast<FFixedUObjectArray*>(ObjObjectsAddr)->GetObjectPtr(Index);

	return nullptr;
}

SDK::int32 SDK::FUObjectArray::Num() const
{
	if (this->bUseChunkArray)
		return reinterpret_cast<FChunkedFixedUObjectArray*>(ObjObjectsAddr)->Num();
	else
		return reinterpret_cast<FFixedUObjectArray*>(ObjObjectsAddr)->Num();

	return 0;
}

SDK::FString SDK::FName::ToString() const
{
	static FString& (*FName__ToString)(const FName * thisPtr, FString & retstr) = nullptr;

	if (!FName__ToString)
		FName__ToString = decltype(FName__ToString)(SDK::Addresses::FName__ToString);

	FString ReturnValue = FName__ToString(this, ReturnValue);
	return ReturnValue;
}

void SDK::FName::ToString(FString& OutString) const
{
	static FString& (*FName__ToStringVoid)(const FName * thisPtr, FString & retstr) = nullptr;

	if (!FName__ToStringVoid)
		FName__ToStringVoid = decltype(FName__ToStringVoid)(SDK::Addresses::FName__ToStringVoid);

	FName__ToStringVoid(this, OutString);
}

SDK::UStruct* SDK::UStruct::SuperStruct() const
{
	static int SuperStructOffset = SDK::Addresses::MemberOffsets::UStruct__SuperStruct;
	return *reinterpret_cast<UStruct**>(__int64(this) + SuperStructOffset);
}

SDK::UField* SDK::UStruct::Children() const
{
	static int ChildrenOffset = SDK::Addresses::MemberOffsets::UStruct_Children;
	return *reinterpret_cast<UField**>(__int64(this) + ChildrenOffset);
}

SDK::int32 SDK::UStruct::Size() const
{
	static int SizeOffset = SDK::Addresses::MemberOffsets::UStruct_Children + 0x8;
	return *reinterpret_cast<int32*>(__int64(this) + SizeOffset);
}

SDK::int32 SDK::UStruct::MinAlignment() const 
{
	static int MinAlignmentOffset = SDK::Addresses::MemberOffsets::UStruct_MinAllignment;
	return *reinterpret_cast<int32*>(__int64(this) + MinAlignmentOffset);
}

SDK::TArray<uint8_t>& SDK::UStruct::Script() const
{
	return *reinterpret_cast<TArray<uint8>*>(__int64(this) + SDK::Addresses::MemberOffsets::UStruct_Script);
}

SDK::UProperty* SDK::UStruct::PropertyLink() const
{
	static int PropertyLinkOffset = SDK::Addresses::MemberOffsets::UStruct_PropertyLink;

	return *reinterpret_cast<UProperty**>(__int64(this) + PropertyLinkOffset);
}

SDK::UProperty* SDK::UStruct::RefLink() const
{
	static int NextRef = SDK::Addresses::MemberOffsets::UStruct_RefLink;

	return *reinterpret_cast<UProperty**>(__int64(this) + NextRef);
}

SDK::UProperty* SDK::UStruct::DestructorLink() const
{
	static int DestructorLinkOffset = SDK::Addresses::MemberOffsets::UStruct_DestructorLink;
	return *reinterpret_cast<UProperty**>(__int64(this) + DestructorLinkOffset);
}

SDK::UProperty* SDK::UStruct::PostConstructLink() const
{
	static int PostConstructLinkOffset = SDK::Addresses::MemberOffsets::UStruct_PostConstructLink;
	return *reinterpret_cast<UProperty**>(__int64(this) + PostConstructLinkOffset);
}

SDK::int32 SDK::UProperty::Offset_Internal()
{
	static int OffsetInternalOffset = SDK::Addresses::MemberOffsets::UProperty__Offset_Internal;
	if (!this)
		return 0;
	return *reinterpret_cast<int32*>(__int64(this) + OffsetInternalOffset);
}

SDK::UProperty* SDK::UProperty::PropertyLinkNext()
{
	static int PropertyLinkNext = SDK::Addresses::MemberOffsets::UProperty__Offset_Internal + 12;
	return *reinterpret_cast<UProperty**>(__int64(this) + PropertyLinkNext);
}

std::string SDK::UProperty::GetPropName()
{
	if (!this) return "INVALID";
	return SDK::UE::GetFortniteVersion() >= 12.1 ? (*(FName*)(__int64(this) + 0x28)).ToString().ToString() : GetName().ToString();
}

SDK::UProperty* SDK::UStruct::FindPropertyByName(std::string PropertyName)
{
	UProperty* result = nullptr;
	result = this->PropertyLink();
	if (!result)
	{
		std::cout << "Failed to get valid PropertyLink from Class: " << this->GetName().ToString() << std::endl;
		return nullptr;
	}
	while (result->GetPropName() != PropertyName)
	{
		result = result->PropertyLinkNext();
		if (!result)
		{
			std::cout << "Failed to get NextProperty from PropertyLink in Class: " << this->GetName().ToString() << std::endl;
			return nullptr;
		}
	}

	return result;
}

void SDK::UObject::ProcessEvent(UFunction* Function, void* Params)
{
	static void (*ProcessEventOriginal)(UObject * thisPtr, UFunction * Function, void* Params) = nullptr;

	if (!ProcessEventOriginal)
		ProcessEventOriginal = decltype(ProcessEventOriginal)(SDK::Addresses::UObject__ProcessEvent);

	return ProcessEventOriginal(this, Function, Params);
}

SDK::UObject* SDK::FUObjectArray::FindObjectFast(std::string Name)
{
	for (int i = 0; i < this->Num(); i++)
	{
		FUObjectItem* ObjectItemPtr = GetObjectPtr(i);

		if (!ObjectItemPtr) continue;

		if (!ObjectItemPtr->Object) continue;

		if (ObjectItemPtr->Object->GetName().ToString() == Name)
			return ObjectItemPtr->Object;
	}

	return nullptr;
}
SDK::UFunction* SDK::UStruct::FindFunctionByName(std::string FunctionName)
{
	for (const UStruct* Class = this; Class; Class = Class->SuperStruct())
	{
		for (UField* Next = Class->Children(); Next != nullptr; Next = Next->Next())
		{
			if (!Next->GetClass()) continue;
			if (Next->GetClass() != SDK::UE::Core::GObjects->FindObjectFast("Function")) continue;
			if (Next->GetName() == FunctionName)
			{
				return reinterpret_cast<UFunction*>(Next);
			}
		}
	}

	return nullptr;
}

SDK::UFunction::FNativeFuncPtr& SDK::UFunction::Func()
{
	return *reinterpret_cast<FNativeFuncPtr*>(__int64(this) + SDK::Addresses::MemberOffsets::UFunction__Func);
}

SDK::UObject* SDK::UClass::GetDefaultObj()
{
	std::string Name = "Default__" + this->GetName().ToString();
	std::cout << Name << std::endl;
	return SDK::UE::Core::GObjects->FindObjectFast(Name);
}

bool SDK::UObjectBaseUtility::IsDefaultObject()
{
	return (GetFlags() & EObjectFlags::RF_ClassDefaultObject);
}

bool SDK::UClass::IsChildOf(const SDK::UStruct* Base) const
{
	if (!Base)
		return false;

	if (this == Base)
		return true;

	for (const UStruct* Struct = this; Struct; Struct = Struct->SuperStruct())
	{
		if (Struct == Base)
			return true;
	}

	return false;
}

SDK::uint8 SDK::UBoolProperty::FieldMask()
{
	if (SDK::UE::GetEngineVersion() <= 4.24 || SDK::UE::GetFortniteVersion() >= 20)
		return *(uint8_t*)(__int64(this) + (112 + 3));
	else if (SDK::UE::GetEngineVersion() >= 4.25)
		return *(uint8_t*)(__int64(this) + (120 + 3));
}

bool SDK::UBoolProperty::ReadBitFieldValue(UObject* Object)
{
	auto Addr = (void*)((PlaceholderBitfield*)(__int64(Object) + this->Offset_Internal()));

	auto BitField = (PlaceholderBitfield*)Addr;

	switch (FieldMask())
	{
	case 0x1:
		return BitField->First;
	case 0x2:
		return BitField->Second;
	case 0x4:
		return BitField->Third;
	case 0x8:
		return BitField->Fourth;
	case 0x10:
		return BitField->Fifth;
	case 0x20:
		return BitField->Sixth;
	case 0x40:
		return BitField->Seventh;
	case 0x80:
		return BitField->Eighth;
	case 0xFF:
		return *(bool*)BitField;
	}

	return false;
}