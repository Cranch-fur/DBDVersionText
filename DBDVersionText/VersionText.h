#pragma once
#include "Utilities.h"






class VersionText
{
private:
	static inline const std::wstring commandLineArgument = L"-versionText=";
	static inline std::wstring customVersionText = L"Use \"" + commandLineArgument + L"\" command line argument to set version text";

	static inline Utilities::Game::E_GameState lastGameState = Utilities::Game::E_GameState::None;
	static inline bool isVersionTextExpected = true;

	static void SetVersionText();
	static DWORD WINAPI ThreadSafeWorker();


public:
	static void Init();
};

