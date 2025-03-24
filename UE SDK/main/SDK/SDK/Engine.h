#pragma once
namespace SDK
{
	class UGameViewportClient;
	class UWorld;
	class AFortGameStateAthena;

	class UEngine : public UObject
	{
	public:
		UGameViewportClient* GameViewport();
	};

	class UGameViewportClient : public UObject
	{
	public:
	};

	class UWorld : public UObject
	{
	public:
		AFortGameStateAthena* GameState();
	};

	class AActor : public UObject
	{
	public:

	};
}