#pragma once
#include "..\UnrealContainers.h"

namespace SDK
{
	using namespace UC;
	class UClass;
	class UObject;
	class FName;
	class UProperty;
	class UFunction;

	class FOutputDevice
	{
	public:

	};

	struct PlaceholderBitfield
	{
		uint8_t First : 1;
		uint8_t Second : 1;
		uint8_t Third : 1;
		uint8_t Fourth : 1;
		uint8_t Fifth : 1;
		uint8_t Sixth : 1;
		uint8_t Seventh : 1;
		uint8_t Eighth : 1;
	};

	enum EObjectFlags
	{
		F_NoFlags = 0x00000000,
		RF_Public = 0x00000001,		// Object is visible outside its package.
		RF_Standalone = 0x00000002,		// Keep object around for editing even if unreferenced.
		RF_MarkAsNative = 0x00000004,		// Object (UField) will be marked as native on construction (DO NOT USE THIS FLAG in HasAnyFlags() etc)
		RF_Transactional = 0x00000008,		// Object is transactional.
		RF_ClassDefaultObject = 0x00000010,		// This object is its class's default object
		RF_ArchetypeObject = 0x00000020,		// This object is a template for another object - treat like a class default object
		RF_Transient = 0x00000040,		// Don't save object.
		RF_MarkAsRootSet = 0x00000080,		// Object will be marked as root set on construction
		RF_TagGarbageTemp = 0x00000100,		// Temporary - only used during garbage collection
		RF_NeedInitialization = 0x00000200,		// Need InitProperties called on object
		RF_NeedLoad = 0x00000400,		// Object needs load.
		RF_KeepForCooker = 0x00000800,		// Keep this object during garbage collection because it's still being used by the cooker
		RF_NeedPostLoad = 0x00001000,		// Object needs PostLoad called on it
		RF_NeedPostLoadSubobjects = 0x00002000,		// During load, indicates that the object still needs to instance subobjects and fixup serialized component references
		RF_NewerVersionExists = 0x00004000,		// Object has been consigned to oblivion due to a newer version of it existing
		RF_BeginDestroyed = 0x00008000,		// BeginDestroy has been called on the object.
		RF_FinishDestroyed = 0x00010000,		// FinishDestroy has been called on the object.
		RF_BeingRegenerated = 0x00020000,		// Object is being regenerated
		RF_DefaultSubObject = 0x00040000,		// This object is a default subobject (this flag is checked in the editor, only get() is checked in the game)
		RF_WasLoaded = 0x00080000,		// This object was loaded
		RF_TextExportTransient = 0x00100000,		// This object should not be included in text form of the object (e.g. copy/paste)
		RF_LoadCompleted = 0x00200000,		// Object has been completely serialized by linker load.
		RF_InheritableComponentTemplate = 0x00400000,		// Archetype of the object can be in its super class
		RF_DuplicateTransient = 0x00800000,		// Object should not be included in any type of duplication (copy/paste, binary duplication, etc.)
		RF_StrongRefOnFrame = 0x01000000,		// References to this object from persistent function frame are handled as strong ones.
		RF_NonPIEDuplicateTransient = 0x02000000,		// Object should not be included for duplication unless it's being duplicated for a PIE session
		RF_WillBeLoaded = 0x08000000,
		RF_HasExternalPackage = 0x10000000,		// This package contains references to objects in external packages.
		RF_MirroredGarbage = 0x40000000,		// This object is modified in memory, and not in the source data.
		RF_AllocatedInSharedPage = 0x80000000,		// This object is being allocated in a shared memory page, so will be in a shared memory pool.
	};

	/**
	* Enum which specifies the mode in which full object names are constructed
	*/
	enum class EObjectFullNameFlags
	{
		// Standard object full name (i.e. "Type PackageName.ObjectName:SubobjectName")
		None = 0,

		// Adds package to the type portion (i.e. "TypePackage.TypeName PackageName.ObjectName:SubobjectName")
		IncludeClassPackage = 1,
	};

	class FName
	{
	public:

		FName() : Index(0), Number(0) {}
		FName(int32 InNumber) : Index(0), Number(InNumber) {}
		FName(int32 InIndex, int32 InNumber) : Index(InIndex), Number(InNumber) {}

		FORCEINLINE int32 GetComparisonIndex() const
		{
			return Index;
		}

		FORCEINLINE int32 GetNumber() const
		{
			return Number;
		}

		FORCEINLINE void SetNumber(const int32 NewNumber)
		{
			Number = NewNumber;
		}

		FString ToString() const;

		void ToString(FString& Out) const;

		FORCEINLINE bool operator==(const FName& Other) const
		{
			return Index == Other.Index && GetNumber() == Other.GetNumber();
		}

		FORCEINLINE bool operator!=(FName Other) const
		{
			return !(*this == Other);
		}

	private:
		uint32 Index;
		uint32 Number;
	};

	class UObjectBase
	{
	public:
		void** VTable;
	public:
		FORCEINLINE EObjectFlags& GetFlags()
		{
			return ObjectFlags;
		}
		/**
		* Returns the unique ID of the object...these are reused so it is only unique while the object is alive
		* Useful as a tag.
		*/
		FORCEINLINE uint32 GetUniqueID() const
		{
			return InternalIndex;
		}

		FORCEINLINE UClass* GetClass() const
		{
			return ClassPrivate;
		}

		FORCEINLINE UObject* GetOuter() const
		{
			return OuterPrivate;
		}

		FORCEINLINE FName GetFName()
		{
			return NamePrivate;
		}

	private:

		EObjectFlags ObjectFlags;
		/** Index into GObjectArray...only valid at runtime */
		uint32 InternalIndex;
		/** Class the object belongs to. */
		UClass* ClassPrivate;
		/** Name of this object */
		FName NamePrivate;
		/** Object this object resides in */
		UObject* OuterPrivate;
	};

	class UObjectBaseUtility : public UObjectBase
	{
	private:
		template<typename ClassType>
		static FORCEINLINE bool IsChildOfWorkaround(const ClassType* ObjClass, const ClassType* TestCls)
		{
			return ObjClass->IsChildOf(TestCls);
		}
	public:
		FORCEINLINE FString GetName()
		{
			return GetFName().ToString();
		}

		FORCEINLINE void GetName(FString& ResultString) 
		{
			GetFName().ToString(ResultString);
		}

		template<typename OtherClassType>
		FORCEINLINE bool IsA(OtherClassType SomeBase) const
		{
			const UClass* SomeBaseClass = SomeBase;
			const UClass* ThisClass = GetClass();

			return IsChildOfWorkaround(ThisClass, SomeBaseClass);
		}

		template<typename Class>
		bool IsA() const
		{
			return IsA(Class::StaticClass());
		}

		bool IsDefaultObject();
	};

	class UObject : public UObjectBaseUtility
	{
	public:
		void ProcessEvent(UFunction* Function, void* Parms);
	};

	class UField : public UObject
	{
	public:
		UField* Next();
	};

	class UStruct : public UField
	{
	public:
		UStruct* SuperStruct() const;
		UField* Children() const;
		int32 Size() const;
		int32 MinAlignment() const;
		TArray<uint8_t>& Script() const;
		UProperty* PropertyLink() const;
		UProperty* RefLink() const;
		UProperty* DestructorLink() const;
		UProperty* PostConstructLink() const;

	public:
		UProperty* FindPropertyByName(std::string PropertyName, bool bUseNext = false);
		UFunction* FindFunctionByName(std::string FunctionName);
	};

	class UProperty : public UField
	{
	public:
		int32 Offset_Internal();
		UProperty* PropertyLinkNext();
	public:
		std::string GetPropName();
	};

	class UBoolProperty : public UProperty
	{
	public:
		uint8 FieldMask();
		bool ReadBitFieldValue(UObject* Object);
	};

	class UClass : public UStruct
	{
	public:
		UObject* GetDefaultObj();
	public:
		bool IsChildOf(const UStruct* Base) const;
	};

	class UFunction : public UStruct
	{
	public:
		using FNativeFuncPtr = void(*)(UObject* Context, void* TheStack, void* Result);

		FNativeFuncPtr& Func();
	};

	struct FUObjectItem
	{
		UObject* Object;

		int32 Flags;

		int32 ClusterRootIndex;
		
		int32 SerialNumber;

		FUObjectItem()
			: Object(nullptr)
			, Flags(0)
			, ClusterRootIndex(0)
			, SerialNumber(0)
		{
		}
		~FUObjectItem()
		{
		}

		FUObjectItem(FUObjectItem&&) = delete;
		FUObjectItem(const FUObjectItem&) = delete;
		FUObjectItem& operator=(FUObjectItem&&) = delete;
		FUObjectItem& operator=(const FUObjectItem&) = delete;

		FORCEINLINE void SetOwnerIndex(int32 OwnerIndex)
		{
			ClusterRootIndex = OwnerIndex;
		}

		FORCEINLINE int32 GetOwnerIndex() const
		{
			return ClusterRootIndex;
		}

		FORCEINLINE void SetClusterIndex(int32 ClusterIndex)
		{
			ClusterRootIndex = -ClusterIndex - 1;
		}

		FORCEINLINE int32 GetClusterIndex() const
		{
			if (ClusterRootIndex < 0)
				return 0;
			return -ClusterRootIndex - 1;
		}

		FORCEINLINE int32 GetSerialNumber() const
		{
			return SerialNumber;
		}

		FORCEINLINE void ResetSerialNumberAndFlags()
		{
			Flags = 0;
			ClusterRootIndex = 0;
			SerialNumber = 0;
		}
	};

	class FFixedUObjectArray
	{
		FUObjectItem* Objects;
		int32 MaxElements;
		int32 NumElements;

	public:
		FORCEINLINE FUObjectItem const* GetObjectPtr(int32 Index) const
		{
			if (Index >= 0 && Index < NumElements)
			{
				return &Objects[Index];
			}

		}

		FORCEINLINE FUObjectItem* GetObjectPtr(int32 Index)
		{
			if (Index >= 0 && Index < NumElements)
			{
				return &Objects[Index];
			}
		}

		/**
		* Return the number of elements in the array
		* Thread safe, but you know, someone might have added more elements before this even returns
		* @return	the number of elements in the array
		**/
		FORCEINLINE int32 Num() const
		{
			return NumElements;
		}

		/**
		* Return the number max capacity of the array
		* Thread safe, but you know, someone might have added more elements before this even returns
		* @return	the maximum number of elements in the array
		**/
		FORCEINLINE int32 Capacity() const
		{
			return MaxElements;
		}

		/**
		* Return if this index is valid
		* Thread safe, if it is valid now, it is valid forever. Other threads might be adding during this call.
		* @param	Index	Index to test
		* @return	true, if this is a valid
		**/
		FORCEINLINE bool IsValidIndex(int32 Index) const
		{
			return Index < Num() && Index >= 0;
		}

		/**
		* Return a reference to an element
		* @param	Index	Index to return
		* @return	a reference to the pointer to the element
		* Thread safe, if it is valid now, it is valid forever. This might return nullptr, but by then, some other thread might have made it non-nullptr.
		**/
		FORCEINLINE FUObjectItem const& operator[](int32 Index) const
		{
			FUObjectItem const* ItemPtr = GetObjectPtr(Index);
			if (ItemPtr)
			{
				return *ItemPtr;
			}
		}

		FORCEINLINE FUObjectItem& operator[](int32 Index)
		{
			FUObjectItem* ItemPtr = GetObjectPtr(Index);
			if (ItemPtr)
			{
				return *ItemPtr;
			}
		}

		/**
		* Return a naked pointer to the fundamental data structure for debug visualizers.
		**/
		UObject*** GetRootBlockForDebuggerVisualizers()
		{
			return nullptr;
		}
	};

	class FChunkedFixedUObjectArray
	{
		enum
		{
			NumElementsPerChunk = 64 * 1024,
		};

		FUObjectItem** Objects;
		FUObjectItem* PreAllocatedObjects;
		int32 MaxElements;
		int32 NumElements;
		int32 MaxChunks;
		int32 NumChunks;
	public:
		/**
		* Return the number of elements in the array
		* Thread safe, but you know, someone might have added more elements before this even returns
		* @return	the number of elements in the array
		**/
		FORCEINLINE int32 Num() const
		{
			return NumElements;
		}

		/**
		* Return the number max capacity of the array
		* Thread safe, but you know, someone might have added more elements before this even returns
		* @return	the maximum number of elements in the array
		**/
		FORCEINLINE int32 Capacity() const
		{
			return MaxElements;
		}

		/**
		* Return if this index is valid
		* Thread safe, if it is valid now, it is valid forever. Other threads might be adding during this call.
		* @param	Index	Index to test
		* @return	true, if this is a valid
		**/
		FORCEINLINE bool IsValidIndex(int32 Index) const
		{
			return Index < Num() && Index >= 0;
		}

		/**
		* Return a pointer to the pointer to a given element
		* @param Index The Index of an element we want to retrieve the pointer-to-pointer for
		**/
		FORCEINLINE FUObjectItem const* GetObjectPtr(int32 Index) const
		{
			const int32 ChunkIndex = Index / NumElementsPerChunk;
			const int32 WithinChunkIndex = Index % NumElementsPerChunk;
			if (IsValidIndex(Index))
			{
				if (ChunkIndex < NumChunks)
				{
					if (Index < MaxElements)
					{
						SDK::FUObjectItem* Chunk = Objects[ChunkIndex];
						if (Chunk)
						{
							return Chunk + WithinChunkIndex;
						}
					}

				}

			}


		}
		FORCEINLINE FUObjectItem* GetObjectPtr(int32 Index)
		{
			const int32 ChunkIndex = Index / NumElementsPerChunk;
			const int32 WithinChunkIndex = Index % NumElementsPerChunk;
			if (IsValidIndex(Index))
			{
				if (ChunkIndex < NumChunks)
				{
					if (Index < MaxElements)
					{
						FUObjectItem* Chunk = Objects[ChunkIndex];
						if (Chunk)
						{
							return Chunk + WithinChunkIndex;
						}
					}
				}
			}

		}

		/**
		* Return a reference to an element
		* @param	Index	Index to return
		* @return	a reference to the pointer to the element
		* Thread safe, if it is valid now, it is valid forever. This might return nullptr, but by then, some other thread might have made it non-nullptr.
		**/
		FORCEINLINE FUObjectItem const& operator[](int32 Index) const
		{
			FUObjectItem const* ItemPtr = GetObjectPtr(Index);
			if (ItemPtr)
			{
				return *ItemPtr;
			}
		}
		FORCEINLINE FUObjectItem& operator[](int32 Index)
		{
			FUObjectItem* ItemPtr = GetObjectPtr(Index);
			if (ItemPtr)
			{
				return *ItemPtr;
			}
		}

		/**
		* Return a naked pointer to the fundamental data structure for debug visualizers.
		**/
		FUObjectItem*** GetRootBlockForDebuggerVisualizers()
		{
			return nullptr;
		}

		int64 GetAllocatedSize() const
		{
			return MaxChunks * sizeof(FUObjectItem*) + NumChunks * NumElementsPerChunk * sizeof(FUObjectItem);
		}
	};

	class FUObjectArray
	{
	private:
		uintptr_t ObjObjectsAddr;
		bool bUseChunkArray;

	public:
		FUObjectArray(uintptr_t ObjectAddress, bool bIsChunked);

		class TIterator
		{
		private:
			FUObjectArray* ObjectArray;
			int32 CurrentIndex;

		public:
			TIterator(FUObjectArray* InArray, int32 StartIndex = 0)
				: ObjectArray(InArray), CurrentIndex(StartIndex)
			{
				AdvanceToValid();
			}

			void AdvanceToValid()
			{
				while (CurrentIndex < ObjectArray->Num() && !IsValid())
				{
					++CurrentIndex;
				}
			}

			bool IsValid() const
			{
				return ObjectArray->GetObjectPtr(CurrentIndex) != nullptr;
			}

			// Prefix increment (++It)
			TIterator& operator++()
			{
				++CurrentIndex;
				AdvanceToValid();
				return *this;
			}

			explicit operator bool() const
			{
				return IsValid();
			}

			FUObjectItem* operator*() const
			{
				return ObjectArray->GetObjectPtr(CurrentIndex);
			}

			bool operator!=(const TIterator& Other) const
			{
				return CurrentIndex != Other.CurrentIndex;
			}
		};

		TIterator begin() { return TIterator(this, 0); }
		TIterator end() { return TIterator(this, Num()); }

	public:
		FUObjectItem* GetObjectPtr(int32 Index);

		FUObjectItem const* GetObjectPtr(int32 Index) const;

		int32 Num() const;

		UObject* FindObjectFast(std::string Name);
	};

	struct FSoftObjectPath
	{
	public:
		FName AssetPathName;
		FString SubPathString;
	public:
		static UClass* GetStructClass();
	};

	class FWeakObjectPtr
	{
	public:
		int32                                         ObjectIndex;
		int32                                         ObjectSerialNumber;
	};

	template<typename TObjectId>
	class TPersistentObjectPtr
	{
	public:
		FWeakObjectPtr                                WeakPtr;
		int32                                         TagAtLastTest;
		TObjectId                                     ObjectID;
	};

	class FSoftObjectPtr : public TPersistentObjectPtr<FSoftObjectPath>
	{
	};

	template<typename UEType>
	class TSoftObjectPtr : public FSoftObjectPtr
	{
	public:
	};

	template<typename UEType>
	class TSoftClassPtr : public FSoftObjectPtr
	{
	public:
	};

	template <typename T = UObject*>
	T GET_PROPERTY_VALUE(SDK::UObject* Object, const char* PropertyName)
	{
		static std::unordered_map<std::string, int> OffsetCache;

		if (!Object) return T();

		std::string Key = Object->GetClass()->GetName().ToString() + "::" + PropertyName;

		int Offset = -1;
		auto It = OffsetCache.find(Key);
		if (It != OffsetCache.end())
		{
			Offset = It->second;
		}
		else
		{
			auto Property = Object->GetClass()->FindPropertyByName(PropertyName);
			if (!Property) return T();

			Offset = Property->Offset_Internal();
			if (Offset <= 0) return T();

			OffsetCache[Key] = Offset;
		}

		return *reinterpret_cast<T*>(reinterpret_cast<uintptr_t>(Object) + Offset);
	}

	template <typename T = UObject*>
	T GET_PROPERTY_OFFSET(SDK::UObject* Object, const char* PropertyName)
	{
		static std::unordered_map<std::string, int> OffsetCache;

		if (!Object) return T();

		std::string Key = Object->GetClass()->GetName().ToString() + "::" + PropertyName;

		int Offset = -1;
		auto It = OffsetCache.find(Key);
		if (It != OffsetCache.end())
		{
			Offset = It->second;
		}
		else
		{
			auto Property = Object->GetClass()->FindPropertyByName(PropertyName);
			if (!Property) return T();

			Offset = Property->Offset_Internal();
			if (Offset <= 0) return T();

			OffsetCache[Key] = Offset;
		}

		return *reinterpret_cast<T*>(reinterpret_cast<uintptr_t>(Object) + Offset);
	}

	template <typename V>
	int GET_PROPERTYSTRUCT_OFFSET(V* Object, const char* PropertyName)
	{
		static std::unordered_map<std::string, int> OffsetCache;

		if (!Object) return 0;

		std::string Key = Object->GetScriptClass()->GetName().ToString() + "::" + PropertyName;

		int Offset = -1;
		auto It = OffsetCache.find(Key);
		if (It != OffsetCache.end())
		{
			Offset = It->second;
		}
		else
		{
			auto Property = Object->GetScriptClass()->FindPropertyByName(PropertyName,true);
			if (!Property) return 0;

			Offset = Property->Offset_Internal();
			if (Offset <= 0) return 0;

			OffsetCache[Key] = Offset;
		}

		return Offset;
	}
}

