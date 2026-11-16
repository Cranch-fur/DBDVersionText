#include "VersionText.h"






static bool IsObjectAlive(const SDK::UObject* object)
{
	const uint32_t deadFlags = static_cast<uint32_t>(SDK::EObjectFlags::BeginDestroyed) | static_cast<uint32_t>(SDK::EObjectFlags::FinishDestroyed);
	return object != nullptr && (static_cast<uint32_t>(object->Flags) & deadFlags) == 0;
}

/* Returns the splash screen, re-using the last one found as long as it's still alive in GObjects. */
SDK::USplashScreen* VersionText::GetCachedSplashScreen()
{
	if (cachedSplashScreen != nullptr && cachedSplashScreenIndex >= 0 &&
		cachedSplashScreenIndex < SDK::UObject::GObjects->Num() &&
		SDK::UObject::GObjects->GetByIndex(cachedSplashScreenIndex) == cachedSplashScreen &&
		IsObjectAlive(cachedSplashScreen))
	{
		return cachedSplashScreen;
	}

	cachedSplashScreen = nullptr;
	cachedSplashScreenIndex = -1;

	int32_t objectsNum = SDK::UObject::GObjects->Num();
	for (int32_t i = 0; i < objectsNum; i++)
	{
		SDK::UObject* object = SDK::UObject::GObjects->GetByIndex(i);
		if (object == nullptr || object->IsDefaultObject() || !IsObjectAlive(object))
			continue;

		if (object->IsA(SDK::USplashScreen::StaticClass()))
		{
			cachedSplashScreen = static_cast<SDK::USplashScreen*>(object);
			cachedSplashScreenIndex = object->Index;
			break;
		}
	}

	return cachedSplashScreen;
}

void VersionText::ResetSplashScreenState()
{
	hasLoggedApply = false;
	lastApplyTick = 0;
	cachedSplashScreen = nullptr;
	cachedSplashScreenIndex = -1;
}

void VersionText::SetVersionText()
{
	Utilities::Game::E_GameState gameState = Utilities::Game::GetGameState();

	if (gameState != Utilities::Game::E_GameState::StartScreen)
	{
		if (gameState != lastGameState)
			ResetSplashScreenState(); // next time we come back, start over.

		lastGameState = gameState;
		return;
	}

	lastGameState = gameState;

	SDK::USplashScreen* splashScreen = GetCachedSplashScreen();
	if (splashScreen == nullptr)
		return;

	ULONGLONG now = GetTickCount64();
	if (lastApplyTick != 0 && now - lastApplyTick < REAPPLY_INTERVAL_MS)
		return;
	lastApplyTick = now;

	splashScreen->SetVersionText(SDK::FString(customVersionText.c_str()));

	if (!hasLoggedApply)
	{
		wprintf(L"[DBDVersionText] splashScreen->SetVersionText(%ls) [game thread]\n", customVersionText.c_str());
		hasLoggedApply = true;
	}
}

void VersionText::SafeUpdate()
{
	__try
	{
		SetVersionText();
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		printf("[DBDVersionText] Exception inside SetVersionText(), skipping this update\n");
		lastUpdateTick = GetTickCount64() + 3000; // back off for ~3s.
	}
}




void __fastcall VersionText::hkProcessEvent(const SDK::UObject* object, SDK::UFunction* function, void* parms)
{
	/* Let the game do its thing first, so we run between the game's own calls, not in the middle of one. */
	oProcessEvent(object, function, parms);

	/* Only the game thread, never re-enter (our own SDK calls go through ProcessEvent too). */
	if (isInsideUpdate || GetCurrentThreadId() != gameThreadId)
		return;

	ULONGLONG now = GetTickCount64();
	if (now < lastUpdateTick + 100)
		return;
	lastUpdateTick = now;

	isInsideUpdate = true;
	SafeUpdate();
	isInsideUpdate = false;
}




/* UE runs its game thread on the process' main thread on Windows - that's the thread that was created first. */
DWORD VersionText::FindMainThreadId()
{
	DWORD pid = GetCurrentProcessId();
	DWORD bestThreadId = 0;
	ULONGLONG bestCreationTime = ~0ull;

	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
	if (snapshot == INVALID_HANDLE_VALUE)
		return 0;

	THREADENTRY32 entry{};
	entry.dwSize = sizeof(entry);
	if (Thread32First(snapshot, &entry))
	{
		do
		{
			if (entry.th32OwnerProcessID != pid)
				continue;

			HANDLE hThread = OpenThread(THREAD_QUERY_LIMITED_INFORMATION, FALSE, entry.th32ThreadID);
			if (!hThread)
				continue;

			FILETIME creation{}, exitTime{}, kernel{}, user{};
			if (GetThreadTimes(hThread, &creation, &exitTime, &kernel, &user))
			{
				ULONGLONG t = (static_cast<ULONGLONG>(creation.dwHighDateTime) << 32) | creation.dwLowDateTime;
				if (t != 0 && t < bestCreationTime)
				{
					bestCreationTime = t;
					bestThreadId = entry.th32ThreadID;
				}
			}
			CloseHandle(hThread);
		} while (Thread32Next(snapshot, &entry));
	}

	CloseHandle(snapshot);
	return bestThreadId;
}




void VersionText::Init()
{
	printf("[DBDVersionText] Starting initialization sequence...\n");

	/* Plain memory reads only - no ProcessEvent calls from this thread. */
	while (SDK::UObject::GObjects == nullptr || SDK::UObject::GObjects->Num() == 0 || SDK::UWorld::GetWorld() == nullptr)
		Sleep(50);

	printf("[DBDVersionText] Game is initialized, looking up command line...\n");
	std::vector<std::wstring> processArguments = Utilities::CommandLine::GetArgumentsUtf16();
	bool hasVersionArgument = false;
	for (std::wstring& arg : processArguments)
	{
		if (Utilities::String::StartsWith(arg, commandLineArgument))
		{
			wprintf(L"[DBDVersionText] Found command line argument \"%ls\"\n", arg.c_str());

			std::vector<std::wstring> split = Utilities::String::Split(arg, L'=', false);
			if (split.size() >= 2)
			{
				customVersionText = split[1];
				hasVersionArgument = true;
			}
			else
				printf("[DBDVersionText] Incorrect command line argument\n");
		}
	}

	if (hasVersionArgument == false)
		printf("[DBDVersionText] Failed to locate command line argument\n");

	wprintf(L"[DBDVersionText] customVersionText: \"%ls\"\n", customVersionText.c_str());

	gameThreadId = FindMainThreadId();
	printf("[DBDVersionText] Game thread id: %lu\n", gameThreadId);
	if (gameThreadId == 0)
	{
		printf("[DBDVersionText] Couldn't determine the game thread, aborting\n");
		return;
	}

	/* customVersionText is fully set before the hook goes live, so the game thread never sees it change. */
	void* processEventAddress = reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(GetModuleHandle(nullptr)) + SDK::Offsets::ProcessEvent);

	MH_STATUS initStatus = MH_Initialize();
	if (initStatus != MH_OK && initStatus != MH_ERROR_ALREADY_INITIALIZED)
	{
		printf("[DBDVersionText] MH_Initialize failed\n");
		return;
	}

	if (MH_CreateHook(processEventAddress, reinterpret_cast<void*>(&hkProcessEvent), reinterpret_cast<void**>(&oProcessEvent)) != MH_OK ||
		MH_EnableHook(processEventAddress) != MH_OK)
	{
		printf("[DBDVersionText] Failed to hook ProcessEvent at 0x%p\n", processEventAddress);
		return;
	}

	printf("[DBDVersionText] ProcessEvent hooked, version text will be applied on the game thread\n");
}
