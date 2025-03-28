#pragma once

namespace SDK
{
	class AFortGameStateAthena : public AActor
	{
	public:

	};

	struct FPlaylistPropertyArray
	{
	public:
		UObject* BasePlaylist();
	public:
		UClass* GetScriptClass();
	};

	struct UIExtensionClass
	{
	public:
		enum class EPlaylistUIExtensionSlot : uint8
		{
			Primary = 0,
			TopRightCorner = 1,
			GameInfoBox = 2,
			Quickbar = 3,
			UpperCenter = 4,
			CrosshairRight = 5,
			UnderSquadInfo = 6,
			FullScreenMap = 7,
			BelowRespawnWidget = 8,
			BelowCompass = 9,
			UnderTeammateStatus = 10,
			ControllerBindingCallout = 11,
			AboveStormMessageSlot = 12,
			CustomMinigameCallouts = 13,
			UnderLocalPlayerInfo = 14,
			InventoryScreenReplacement = 15,
			Reticle = 16,
			KillfeedSlot = 17,
			PrioritizedContextualSlot = 18,
			EPlaylistUIExtensionSlot_MAX = 19,
		};

		EPlaylistUIExtensionSlot Slot;
		uint8                                         Pad_1[0x7];
		TSoftClassPtr<UClass> WidgetClass;
	public:
		static UClass* GetScriptClass();
	};
}