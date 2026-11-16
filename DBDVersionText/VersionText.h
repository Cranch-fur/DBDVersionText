#pragma once
#include "Utilities.h"
#include "MinHook.h"

#include <TlHelp32.h>






class VersionText
{
private:
	static inline const std::wstring commandLineArgument = L"-versionText=";
	static inline std::wstring customVersionText = L"Use \"" + commandLineArgument + L"\" command line argument to set version text";

	static inline Utilities::Game::E_GameState lastGameState = Utilities::Game::E_GameState::None;

	/* Splash screen lookup is cached so re-applying never has to scan GObjects. */
	static inline SDK::USplashScreen* cachedSplashScreen = nullptr;
	static inline int32_t cachedSplashScreenIndex = -1;
	static inline bool hasLoggedApply = false;

	/* While on the StartScreen our text is re-applied this often, in case the game writes its own text after us. */
	static inline const ULONGLONG REAPPLY_INTERVAL_MS = 250;
	static inline ULONGLONG lastApplyTick = 0;

	static SDK::USplashScreen* GetCachedSplashScreen();
	static void ResetSplashScreenState();

	/* Game-thread hook state */
	using ProcessEvent_t = void(__fastcall*)(const SDK::UObject*, SDK::UFunction*, void*);
	static inline ProcessEvent_t oProcessEvent = nullptr;
	static inline DWORD gameThreadId = 0;
	static inline ULONGLONG lastUpdateTick = 0;
	static inline bool isInsideUpdate = false;   // only touched on the game thread.

	static void SetVersionText();
	static void SafeUpdate();
	static DWORD FindMainThreadId();
	static void __fastcall hkProcessEvent(const SDK::UObject* object, SDK::UFunction* function, void* parms);


public:
	static void Init();
};
