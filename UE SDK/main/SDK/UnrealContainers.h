#pragma once
#include <string>
#include <stdexcept>



namespace UC
{
	typedef uint32_t uint32;
	typedef	uint16_t uint16;
	typedef	uint8_t uint8;
	typedef	uint64_t uint64;
	typedef	int32_t int32;
	typedef	int16_t int16;
	typedef	int8_t int8;
	typedef	int64_t int64;

	/** Chooses between two different classes based on a boolean. */
	template<bool Predicate, typename TrueClass, typename FalseClass>
	class TChooseClass;

	template<typename TrueClass, typename FalseClass>
	class TChooseClass<true, TrueClass, FalseClass>
	{
	public:
		typedef TrueClass Result;
	};

	template<typename TrueClass, typename FalseClass>
	class TChooseClass<false, TrueClass, FalseClass>
	{
	public:
		typedef FalseClass Result;
	};

	FORCEINLINE size_t DefaultQuantizeSize(size_t Count, uint32_t Alignment)
	{
		return Count;
	}

	FORCEINLINE int32_t DefaultCalculateSlackReserve(int32_t NumElements, size_t BytesPerElement, bool bAllowQuantize, uint32_t Alignment = 8)
	{
		int32_t Retval = NumElements;
		if (bAllowQuantize)
		{
			Retval = int32_t(DefaultQuantizeSize(size_t(Retval) * size_t(BytesPerElement), Alignment) / BytesPerElement);

			if (NumElements > Retval)
			{
				Retval = INT32_MAX;
			}
		}

		return Retval;
	}

	inline int32 Min(int32 A, int32 B)
	{
		bool v10 = A <= B;

		int ret = B;

		if (v10)
			ret = A;
		return ret;
	}

	inline int32 CalculateSlackReserve(int32 NewMax, SIZE_T NumBytesPerElements)
	{
		return DefaultCalculateSlackReserve(NewMax, NumBytesPerElements, false);
	}

	FORCEINLINE int32_t DefaultCalculateSlackGrow(int32_t NumElements, int32_t NumAllocatedElements, size_t BytesPerElement, bool bAllowQuantize, uint32_t Alignment = 8)
	{
		const size_t FirstGrow = 4;
		const size_t ConstantGrow = 16;
		int32_t Retval;

		if (NumElements > NumAllocatedElements && NumElements > 0) return 0;

		size_t Grow = FirstGrow;

		if (NumAllocatedElements || size_t(NumElements) > Grow)
		{
			Grow = size_t(NumElements) + 3 * size_t(NumElements) / 8 + ConstantGrow;
		}
		if (bAllowQuantize)
		{
			Retval = int32_t(DefaultQuantizeSize(Grow * BytesPerElement, Alignment) / BytesPerElement);
		}
		else
		{
			Retval = int32_t(Grow);
		}
		if (NumElements > Retval)
		{
			Retval = INT32_MAX;
		}

		return Retval;
	}

	class FMemory
	{
	public:
		static void* Realloc(void* Original, uint64 Size, uint32 Alignment)
		{

			return realloc(Original, Size); //Allignment Just not needed ????????????????????
		}

		static void* Free(void* ptr)
		{
			return realloc(ptr, 0);
		}
	};

	template<typename InElementType>
	class TArray
	{
		friend class FString;

	public:
		typedef InElementType ElementType;

		/**
		* Constructor, initializes element number counters.
		*/

		FORCEINLINE TArray()
			:ArrayNum(0)
			, ArrayMax(0)
		{
		}

	protected:
		ElementType* Data;
		int32 ArrayNum;
		int32 ArrayMax;

	public:
		/**
		* Helper function for returning a typed pointer to the first array entry.
		*
		* @returns Pointer to first array entry or nullptr if ArrayMax == 0.
		*/
		FORCEINLINE ElementType* GetData()
		{
			return (ElementType*)Data;
		}

		FORCEINLINE const ElementType& GetData() const
		{
			return (const ElementType&)Data;
		}

		FORCEINLINE ElementType& GetFirstData()
		{
			return Data[0];
		}

		/**
		* Helper function for returning a typed pointer to the first array entry.
		*
		* @returns Pointer to first array entry or nullptr if ArrayMax == 0.
		*/
		FORCEINLINE const ElementType& GetFirstData() const
		{
			return (const ElementType&)Data[0];
		}

		FORCEINLINE ElementType& GetData(int32 Index)
		{
			return Data[Index];
		}

		FORCEINLINE const ElementType& GetData(int32 Index) const
		{
			return (const ElementType&)Data[Index];
		}

		/**
		* Helper function returning the size of the inner type.
		*
		* @returns Size in bytes of array type.
		*/
		FORCEINLINE uint32 GetTypeSize() const
		{
			return sizeof(ElementType);
		}

		/**
		 * Returns the amount of slack in this array in elements.
		 *
		 * @see Num, Shrink
		 */
		FORCEINLINE int32 GetSlack() const
		{
			return ArrayMax - ArrayNum;
		}

		/**
		* Tests if index is valid, i.e. greater than or equal to zero, and less than the number of elements in the array.
		*
		* @param Index Index to test.
		* @returns True if index is valid. False otherwise.
		*/
		FORCEINLINE bool IsValidIndex(int32 Index) const
		{
			return Index >= 0 && Index < ArrayNum;
		}

		/**
	 * Returns number of elements in array.
	 *
	 * @returns Number of elements in array.
	 * @see GetSlack
	 */
		FORCEINLINE int32 Num() const
		{
			return ArrayNum;
		}

		/**
		 * Returns maximum number of elements in array.
		 *
		 * @returns Maximum number of elements in array.
		 * @see GetSlack
		 */
		FORCEINLINE int32 Max() const
		{
			return ArrayMax;
		}

		/**
		 * Array bracket operator. Returns reference to element at give index.
		 *
		 * @returns Reference to indexed element.
		 */
		FORCEINLINE ElementType& operator[](int32 Index)
		{
			return GetData(Index);
		}

		/**
		* Array bracket operator. Returns reference to element at give index.
		*
		* Const version of the above.
		*
		* @returns Reference to indexed element.
		*/
		FORCEINLINE const ElementType& operator[](int32 Index) const
		{
			return GetData(Index);
		}

		/**
		* Finds element within the array.
		*
		* @param Item Item to look for.
		* @param Index Will contain the found index.
		* @returns True if found. False otherwise.
		* @see FindLast, FindLastByPredicate
		*/

		FORCEINLINE bool Find(const ElementType& Item, int32 Index) const
		{
			Index = this->Find(Item);
			return Index != -1;
		}

		/**
		* Finds element within the array.
		*
		* @param Item Item to look for.
		* @returns Index of the found element. INDEX_NONE otherwise.
		* @see FindLast, FindLastByPredicate
		*/
		int32 Find(const ElementType& Item) const
		{
			for (int i = 0; i < ArrayNum; i++)
			{
				if (Data[i] == Item)
				{
					return static_cast<int32>(i);
				}
			}

			return -1;
		}

		FORCEINLINE bool FindLast(const ElementType& Item, int32 Index) const
		{
			Index = this->FindLast(Item);
			return Index != -1;
		}

		int32 FindLast(const ElementType& Item) const
		{
			for (int i = ArrayNum; i > 0; i--)
			{
				if (Data[i] == Item)
				{
					return static_cast<int32>(i);
				}
			}

			return -1;
		}

		bool Contains(const InElementType& Item) const
		{
			for (int i = 0; i < ArrayNum; i++)
			{
				if (Data[i] == Item)
				{
					return true;
				}
			}

			return false;
		}

		FORCEINLINE void ResizeGrow(int32 OldNum)
		{
			ArrayMax = DefaultCalculateSlackGrow(ArrayNum, ArrayMax, sizeof(ElementType), false);
			ElementType* OldData = Data;
			if (ArrayMax)
			{
				Data = (ElementType*)FMemory::Realloc(Data, (ArrayMax = ArrayNum + OldNum) * sizeof(ElementType), alignof(ElementType));

				if (OldData && OldNum)
				{
					const int32 NumCopiedElements = Min(ArrayMax, OldNum);
					memcpy(Data, OldData, NumCopiedElements * sizeof(ElementType));
				}
			}
		}

		FORCEINLINE void ResizeTo(int32 NewMax)
		{
			if (NewMax)
			{
				NewMax = DefaultCalculateSlackReserve(NewMax, sizeof(ElementType), false);
			}
			if (NewMax != ArrayMax)
			{
				ArrayMax = NewMax;

				ElementType* OldData = Data;

				if (ArrayMax)
				{
					Data = (ElementType*)FMemory::Realloc(Data, (ArrayMax = NewMax) * sizeof(ElementType), alignof(ElementType));

					if (OldData && NewMax)
					{
						const int32 NumCompiedElements = Min(NewMax, ArrayNum);
						memcpy(Data, OldData, NumCompiedElements * sizeof(ElementType));
					}
				}
			}
		}

		FORCEINLINE int32 AddUnitalized(int32 Count = 1)
		{
			if (Count >= 0)
			{
				const int32 OldNum = ArrayNum;
				if ((ArrayNum += Count) > ArrayMax)
				{
					ResizeGrow(OldNum);
				}

				return OldNum;
			}
		}

		FORCEINLINE int32 Emplace(InElementType& Item)
		{
			const int32 Index = AddUnitalized(1);
			Data[Index] = Item;

			return Index;
		}

		FORCEINLINE int32 Emplace(const InElementType& Item)
		{
			const int32 Index = AddUnitalized(1);
			Data[Index] = Item;

			return Index;
		}

		FORCEINLINE ElementType& Emplace_GetRef(InElementType&& Item)
		{
			const int32 Index = AddUnitalized(1);
			Data[Index] = Item;
			return Data[Index];
		}

		FORCEINLINE int32 Add(InElementType& Item)
		{
			if (&Item)
				return Emplace(Item);
		}

		FORCEINLINE int32 Add(const ElementType& Item)
		{
			if (&Item)
				return Emplace(&Item);
		}

		FORCEINLINE ElementType& Add_GetRef(ElementType&& Item)
		{
			if (&Item)
			{
				return Emplace_GetRef(Item);
			}
		}

		FORCEINLINE ElementType& Add_GetRef(const ElementType& Item)
		{
			if (&Item)
				return Emplace_GetRef(Item);
		}


		FORCEINLINE void Reserve(int32 Number)
		{
			if (Number >= 0)
			{
				if (Number > ArrayMax)
					ResizeTo(Number);
			}
		}

		inline bool Remove(int32 Index)
		{
			if (!IsValidIndex(Index)) return false;

			ArrayNum--;

			for (int i = Index; i < ArrayNum; i++)
			{
				int32 NextIndex = i + 1;
				Data[i] = Data[NextIndex];
			}
		}

		int32 RemoveSingle(const ElementType& Item)
		{
			int32 Index = Find(Item);

			if (Index == -1)
			{
				return 0;
			}

			--ArrayNum;

			for (int i = Index; i < ArrayNum; i++)
			{
				int32 NextIndex = i + 1;
				Data[i] = Data[NextIndex];
			}

			return 1;
		}

		inline void Free()
		{
			if (Data)
				FMemory::Free(Data);

			ArrayMax = 0;
			ArrayNum = 0;
			Data = nullptr;
		}
	};

	struct FSparseArrayAllocationInfo
	{
		int32 Index;
		void* Pointer;
	};

	template<typename ElementType>
	class TSparseArray;

	/** Allocated elements are overlapped with free element info in the element list. */
	template<typename ElementType>
	union TSparseArrayElementOrFreeListLink
	{
		/** If the element is allocated, its value is stored here. */
		ElementType ElementData;

		struct
		{
			/** If the element isn't allocated, this is a link to the previous element in the array's free list. */
			int32 PrevFreeIndex;

			/** If the element isn't allocated, this is a link to the next element in the array's free list. */
			int32 NextFreeIndex;
		};
	};

	template<typename AllocatorType>
	class TBitArray
	{
	private:
		AllocatorType AllocatorInstance;
		int32         NumBits;
		int32         MaxBits;
	};

	/*fuck that for now*/

	class FString
	{

	private:
		typedef TArray<wchar_t> DataType;
		DataType Data;

	public:
		using ElementType = wchar_t;

		FString() = default;
		FString(FString&&) = default;
		FString(const FString&) = default;
		FString& operator=(FString&&) = default;
		FString& operator=(const FString&) = default;

	public:
		FString(const wchar_t* Str)
		{
			const uint32 NullTerminatedLength = static_cast<uint32>(wcslen(Str) + 0x1);

			Data.Data = (const_cast<wchar_t*>(Str));
			Data.ArrayNum = NullTerminatedLength;
			Data.ArrayMax = NullTerminatedLength;
		}

		FString(const std::string Str)
		{
			std::wstring wstr(Str.begin(), Str.end());

			FString(wstr.c_str());
		}

	public:
		std::string ToString()
		{
			wchar_t* ProcessedData = Data.GetData();
			std::wstring WideDataString(ProcessedData);

			return std::string(WideDataString.begin(), WideDataString.end());
		}

		std::wstring ToWideString()
		{
			wchar_t* ProcessedData = Data.GetData();
			std::wstring WideDataString(ProcessedData);

			return WideDataString;
		}

		FORCEINLINE const TCHAR* operator *() const
		{
			const wchar_t* t = L"";
			return Data.Num() ? &Data.GetData() : t;
		}

		bool operator!=(std::string& Other)
		{
			return this->ToString() != Other;
		}
		bool operator!=(std::wstring& Other)
		{
			return this->ToWideString() != Other;
		}
		bool operator !=(FString& Other)
		{
			return this->Data.Data != Other.Data.Data;
		}
		bool operator !=(const char* Other)
		{
			int result = std::strcmp(this->ToString().c_str(), Other);
			return !!result;
		}
		bool operator !=(const wchar_t* Other)
		{
			int result = std::wcscmp(this->Data.Data, Other);
			return !!result;
		}

		bool operator==(std::string& Other)
		{
			return this->ToString() == Other;
		}
		bool operator==(std::wstring& Other)
		{
			return this->ToWideString() == Other;
		}
		bool operator==(FString& Other)
		{
			return this->Data.Data == Other.Data.Data;
		}
		bool operator==(const char* Other)
		{
			int result = std::strcmp(this->ToString().c_str(), Other);
			return !result;
		}
		bool operator==(const wchar_t* Other)
		{
			int result = std::wcscmp(this->Data.Data, Other);
			return !result;
		}

		friend std::ostream& operator<<(std::ostream& Stream, UC::FString& Str) { return Stream << Str.ToString(); }
		friend std::istream& operator>>(std::istream& Stream, UC::FString& Str) {
			std::string string = Str.ToString();
			return Stream >> string;
		}
		friend std::ostream& operator<<(std::ofstream& Stream, UC::FString& Str)
		{
			return Stream << Str.ToString();
		}
		friend std::ostream& operator<<(std::ostream& Stream, UC::FString Str) { return Stream << Str.ToString(); }
		friend std::istream& operator>>(std::istream& Stream, UC::FString Str) {
			std::string string = Str.ToString();
			return Stream >> string;
		}
		friend std::ostream& operator<<(std::ofstream& Stream, UC::FString Str)
		{
			return Stream << Str.ToString();
		}
	};
}