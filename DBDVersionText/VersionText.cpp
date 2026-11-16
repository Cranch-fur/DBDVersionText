#include "VersionText.h"






void VersionText::SetVersionText()
{
	/* Dead by Daylight 6.7.0 PTB */
	static const size_t SCAN_POS = 0x0C1FB688;
	static const std::vector<uintptr_t> SCAN_OFFSETS =
	{
			0x8,     // Offset 0
			0x8,     // Offset 1
			0x10,    // Offset 2
			0x338,   // Offset 3
			0x10,    // Offset 4
			0x50,    // Offset 5
			0x5D8,   // Offset 6
			0xA0,    // Offset 7
			0x4E0    // Offset 8
	};

	Utilities::Game::E_GameState gameState = Utilities::Game::GetGameState();
	if (isVersionTextExpected && gameState == Utilities::Game::E_GameState::StartScreen)
	{
		HMODULE hMainModule = GetModuleHandle(nullptr);
		uintptr_t startingAddress = Utilities::Memory::Internal::AddressAddOffset(reinterpret_cast<uintptr_t>(hMainModule), SCAN_POS);
		uintptr_t versionTextAddress = Utilities::Memory::Internal::AddressFollowPointerChain(startingAddress, SCAN_OFFSETS);

		if (versionTextAddress)
		{
			printf("[DBDVersionText] Found version text at: 0x%llX\n", (unsigned long long)versionTextAddress);

			std::wstring versionText = Utilities::Memory::Internal::GetWString(versionTextAddress);
			wprintf(L"[DBDVersionText] Current version text: %ls\n", versionText.c_str());

			if (versionText.empty())
			{
				printf("[DBDVersionText] Version text is empty\n");
				return;
			}

			size_t versionTextLength = versionText.length();
			if (versionTextLength < 5)
			{
				printf("[DBDVersionText] Version text is too short\n");
				return;
			}

			SDK::USplashScreen* splashScreen = Utilities::Game::GetSplashScreen();
			if (splashScreen == nullptr)
			{
				printf("[DBDVersionText] Failed to locate SplashScreen\n");
				return;
			}

			splashScreen->SetVersionText(SDK::FString(customVersionText.c_str()));
			wprintf(L"[DBDVersionText] splashScreen->SetVersionText(%ls)\n", customVersionText.c_str());

			isVersionTextExpected = false;
		}
	}
	else if (gameState != lastGameState && (gameState != Utilities::Game::E_GameState::StartScreen))
	{
		isVersionTextExpected = true;
	}

	lastGameState = gameState;
}

DWORD WINAPI VersionText::ThreadSafeWorker()
{
	printf("[DBDVersionText] ThreadSafeWorker() is now entering the loop\n");

	while (true)
	{
		EXCEPTION_POINTERS* exceptionInfo = nullptr;

		__try
		{
			SetVersionText();
			Sleep(100);
		}
		__except (exceptionInfo = GetExceptionInformation(), EXCEPTION_EXECUTE_HANDLER)
		{
			if (exceptionInfo)
			{
				void* crashAddress = exceptionInfo->ExceptionRecord->ExceptionAddress;
				DWORD exceptionCode = exceptionInfo->ExceptionRecord->ExceptionCode;

				HMODULE hModule = GetModuleHandle(nullptr);
				uintptr_t baseAddress = (uintptr_t)hModule;
				uintptr_t relativeOffset = (uintptr_t)crashAddress - baseAddress;

				printf("[DBDVersionText] FATAL ERROR / CRASH in %s\n", __FUNCSIG__);
				printf("[DBDVersionText] Exception Code:   0x%08X\n", exceptionCode);
				printf("[DBDVersionText] Absolute Address: 0x%llX\n", (unsigned long long)crashAddress);
				printf("[DBDVersionText] Module Base:      0x%llX\n", (unsigned long long)baseAddress);
				printf("[DBDVersionText] Relative Offset:  0x%llX\n", (unsigned long long)relativeOffset);
			}

			printf("[DBDVersionText] Restarting loop in 3 seconds...\n");
			Sleep(3000);
		}
	}

	return 0;
}




void VersionText::Init()
{
	printf("[DBDVersionText] Starting initialization sequence...\n");
	while (Utilities::Game::IsInitialized() == false)
	{
		Sleep(10);
	}

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

	printf("[DBDVersionText] Starting ThreadSafeWorker()...\n");
	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)ThreadSafeWorker, 0, 0, 0);
}