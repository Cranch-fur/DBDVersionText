#include "Utilities.h"






std::string Utilities::CommandLine::Get()
{
    if (LPSTR cmdLine = GetCommandLineA())
        return std::string(cmdLine);

    return std::string();
}

std::wstring Utilities::CommandLine::GetUtf16()
{
    if (LPWSTR cmdLine = GetCommandLineW())
        return std::wstring(cmdLine);

    return std::wstring();
}


std::vector<std::string> Utilities::CommandLine::GetArguments()
{
    std::vector<std::string> outArguments;

    std::vector<std::wstring> arguments = GetArgumentsUtf16();
    for (std::wstring& arg : arguments)
    {
        outArguments.push_back(String::ToString(arg));
    }

    return outArguments;
}

std::vector<std::wstring> Utilities::CommandLine::GetArgumentsUtf16()
{
    std::vector<std::wstring> outArguments;
    int32_t argCount = 0;

    if (LPWSTR* argv = CommandLineToArgvW(GetCommandLineW(), &argCount))
    {
        for (int32_t argId = 0; argId < argCount; ++argId)
        {
            if (argv[argId])
                outArguments.push_back(std::wstring(argv[argId]));
        }

        LocalFree(argv);
    }

    return outArguments;
}


int Utilities::CommandLine::Count()
{
    int32_t argCount = 0;

    if (LPWSTR* argv = CommandLineToArgvW(GetCommandLineW(), &argCount))
        LocalFree(argv);

    return argCount;
}


bool Utilities::CommandLine::HasArgument(const std::string& argument)
{
    if (argument.empty())
        return false;

    std::vector<std::string> arguments = GetArguments();
    for (std::string& arg : arguments)
    {
        if (arg == argument)
            return true;
    }

    return false;
}

bool Utilities::CommandLine::HasArgument(const std::wstring& argument)
{
    if (argument.empty())
        return false;

    std::vector<std::wstring> arguments = GetArgumentsUtf16();
    for (std::wstring& arg : arguments)
    {
        if (arg == argument)
            return true;
    }

    return false;
}

bool Utilities::CommandLine::HasArgument(const char* argument)
{
    return HasArgument(String::ToString(argument));
}

bool Utilities::CommandLine::HasArgument(const wchar_t* argument)
{
    return HasArgument(String::ToWString(argument));
}




std::string Utilities::String::ToString(const std::wstring& wideString)
{
    if (wideString.empty())
    {
        return std::string();
    }

    const wchar_t* wcString = wideString.c_str();
    return ToString(wcString);
}

std::string Utilities::String::ToString(const char* cString)
{
    if (cString == nullptr)
    {
        return std::string();
    }

    return std::string(cString);
}

std::string Utilities::String::ToString(const wchar_t* wcString)
{
    if (wcString == nullptr)
    {
        return std::string();
    }

    size_t convertedCount = 0;
    errno_t errorCode = wcstombs_s(&convertedCount, nullptr, 0, wcString, 0);

    if (errorCode != 0 || convertedCount == 0)
    {
        return std::string();
    }

    std::vector<char> buffer(convertedCount);
    errorCode = wcstombs_s(&convertedCount, buffer.data(), buffer.size(), wcString, _TRUNCATE);

    if (errorCode != 0)
    {
        return std::string();
    }

    /* Return string excluding the null terminator. */
    return std::string(buffer.data(), convertedCount - 1);
}


std::wstring Utilities::String::ToWString(const std::string& string)
{
    if (string.empty())
    {
        return std::wstring();
    }

    const char* cString = string.c_str();
    return ToWString(cString);
}

std::wstring Utilities::String::ToWString(const char* cString)
{
    if (cString == nullptr)
    {
        return std::wstring();
    }

    size_t convertedCount = 0;
    errno_t errorCode = mbstowcs_s(&convertedCount, nullptr, 0, cString, 0);

    if (errorCode != 0 || convertedCount == 0)
    {
        return std::wstring();
    }

    std::vector<wchar_t> buffer(convertedCount);
    errorCode = mbstowcs_s(&convertedCount, buffer.data(), buffer.size(), cString, _TRUNCATE);

    if (errorCode != 0)
    {
        return std::wstring();
    }

    /* Return wide string excluding the null terminator. */
    return std::wstring(buffer.data(), convertedCount - 1);
}

std::wstring Utilities::String::ToWString(const wchar_t* wcString)
{
    if (wcString == nullptr)
    {
        return std::wstring();
    }

    return std::wstring(wcString);
}




std::string Utilities::String::ToLowerCase(std::string string)
{
    std::transform(string.begin(), string.end(), string.begin(), [](unsigned char c)
        {
            return std::tolower(c);
        });

    return string;
}

std::wstring Utilities::String::ToLowerCase(std::wstring wString)
{
    std::transform(wString.begin(), wString.end(), wString.begin(), [](wchar_t c)
        {
            return std::towlower(c);
        });

    return wString;
}

std::string Utilities::String::ToLowerCase(const char* cString)
{
    return ToLowerCase(String::ToString(cString));
}

std::wstring Utilities::String::ToLowerCase(const wchar_t* wcString)
{
    return ToLowerCase(String::ToWString(wcString));
}




std::string Utilities::String::ToUpperCase(std::string string)
{
    std::transform(string.begin(), string.end(), string.begin(), [](unsigned char c)
        {
            return std::toupper(c);
        });

    return string;
}

std::wstring Utilities::String::ToUpperCase(std::wstring wString)
{
    std::transform(wString.begin(), wString.end(), wString.begin(), [](wchar_t c)
        {
            return std::towupper(c);
        });

    return wString;
}

std::string Utilities::String::ToUpperCase(const char* cString)
{
    return ToUpperCase(String::ToString(cString));
}

std::wstring Utilities::String::ToUpperCase(const wchar_t* wcString)
{
    return ToUpperCase(String::ToWString(wcString));
}




std::string Utilities::String::Replace(std::string string, const std::string& from, const std::string& to)
{
    if (from.empty())
    {
        return string;
    }

    size_t start_pos = 0;
    while ((start_pos = string.find(from, start_pos)) != std::string::npos)
    {
        string.replace(start_pos, from.length(), to);
        start_pos += to.length();
    }

    return string;
}

std::string Utilities::String::Replace(const std::string& string, const std::string& from, const std::wstring& to)
{
    return Replace(string, from, ToString(to));
}

std::string Utilities::String::Replace(const std::string& string, const std::wstring& from, const std::string& to)
{
    return Replace(string, ToString(from), to);
}

std::string Utilities::String::Replace(const std::string& string, const std::wstring& from, const std::wstring& to)
{
    return Replace(string, ToString(from), ToString(to));
}

std::string Utilities::String::Replace(const char* cString, const char* from, const char* to)
{
    return Replace(ToString(cString), ToString(from), ToString(to));
}


std::wstring Utilities::String::Replace(std::wstring wString, const std::wstring& from, const std::wstring& to)
{
    if (from.empty())
    {
        return wString;
    }

    size_t start_pos = 0;
    while ((start_pos = wString.find(from, start_pos)) != std::wstring::npos)
    {
        wString.replace(start_pos, from.length(), to);
        start_pos += to.length();
    }

    return wString;
}

std::wstring Utilities::String::Replace(const std::wstring& wString, const std::wstring& from, const std::string& to)
{
    return Replace(wString, from, ToWString(to));
}

std::wstring Utilities::String::Replace(const std::wstring& wString, const std::string& from, const std::wstring& to)
{
    return Replace(wString, ToWString(from), to);
}

std::wstring Utilities::String::Replace(const std::wstring& wString, const std::string& from, const std::string& to)
{
    return Replace(wString, ToWString(from), ToWString(to));
}

std::wstring Utilities::String::Replace(const wchar_t* wcString, const wchar_t* from, const wchar_t* to)
{
    return Replace(ToWString(wcString), ToWString(from), ToWString(to));
}




std::string Utilities::String::ReplaceFirstOf(std::string string, const std::string& from, const std::string& to)
{
    if (from.empty())
    {
        return string;
    }

    size_t start_pos = string.find(from);
    if (start_pos != std::string::npos)
    {
        string.replace(start_pos, from.length(), to);
    }

    return string;
}

std::string Utilities::String::ReplaceFirstOf(const std::string& string, const std::string& from, const std::wstring& to)
{
    return ReplaceFirstOf(string, from, ToString(to));
}

std::string Utilities::String::ReplaceFirstOf(const std::string& string, const std::wstring& from, const std::string& to)
{
    return ReplaceFirstOf(string, ToString(from), to);
}

std::string Utilities::String::ReplaceFirstOf(const std::string& string, const std::wstring& from, const std::wstring& to)
{
    return ReplaceFirstOf(string, ToString(from), ToString(to));
}

std::string Utilities::String::ReplaceFirstOf(const char* cString, const char* from, const char* to)
{
    return ReplaceFirstOf(ToString(cString), ToString(from), ToString(to));
}


std::wstring Utilities::String::ReplaceFirstOf(std::wstring wString, const std::wstring& from, const std::wstring& to)
{
    if (from.empty())
    {
        return wString;
    }

    size_t start_pos = wString.find(from);
    if (start_pos != std::wstring::npos)
    {
        wString.replace(start_pos, from.length(), to);
    }

    return wString;
}

std::wstring Utilities::String::ReplaceFirstOf(const std::wstring& wString, const std::wstring& from, const std::string& to)
{
    return ReplaceFirstOf(wString, from, ToWString(to));
}

std::wstring Utilities::String::ReplaceFirstOf(const std::wstring& wString, const std::string& from, const std::wstring& to)
{
    return ReplaceFirstOf(wString, ToWString(from), to);
}

std::wstring Utilities::String::ReplaceFirstOf(const std::wstring& wString, const std::string& from, const std::string& to)
{
    return ReplaceFirstOf(wString, ToWString(from), ToWString(to));
}

std::wstring Utilities::String::ReplaceFirstOf(const wchar_t* wcString, const wchar_t* from, const wchar_t* to)
{
    return ReplaceFirstOf(ToWString(wcString), ToWString(from), ToWString(to));
}


std::string Utilities::String::ReplaceLastOf(std::string string, const std::string& from, const std::string& to)
{
    if (from.empty())
    {
        return string;
    }

    size_t start_pos = string.rfind(from);
    if (start_pos != std::string::npos)
    {
        string.replace(start_pos, from.length(), to);
    }

    return string;
}

std::string Utilities::String::ReplaceLastOf(const std::string& string, const std::string& from, const std::wstring& to)
{
    return ReplaceLastOf(string, from, ToString(to));
}

std::string Utilities::String::ReplaceLastOf(const std::string& string, const std::wstring& from, const std::string& to)
{
    return ReplaceLastOf(string, ToString(from), to);
}

std::string Utilities::String::ReplaceLastOf(const std::string& string, const std::wstring& from, const std::wstring& to)
{
    return ReplaceLastOf(string, ToString(from), ToString(to));
}

std::string Utilities::String::ReplaceLastOf(const char* cString, const char* from, const char* to)
{
    return ReplaceLastOf(ToString(cString), ToString(from), ToString(to));
}


std::wstring Utilities::String::ReplaceLastOf(std::wstring wString, const std::wstring& from, const std::wstring& to)
{
    if (from.empty())
    {
        return wString;
    }

    size_t start_pos = wString.rfind(from);
    if (start_pos != std::wstring::npos)
    {
        wString.replace(start_pos, from.length(), to);
    }

    return wString;
}

std::wstring Utilities::String::ReplaceLastOf(const std::wstring& wString, const std::wstring& from, const std::string& to)
{
    return ReplaceLastOf(wString, from, ToWString(to));
}

std::wstring Utilities::String::ReplaceLastOf(const std::wstring& wString, const std::string& from, const std::wstring& to)
{
    return ReplaceLastOf(wString, ToWString(from), to);
}

std::wstring Utilities::String::ReplaceLastOf(const std::wstring& wString, const std::string& from, const std::string& to)
{
    return ReplaceLastOf(wString, ToWString(from), ToWString(to));
}

std::wstring Utilities::String::ReplaceLastOf(const wchar_t* wcString, const wchar_t* from, const wchar_t* to)
{
    return ReplaceLastOf(ToWString(wcString), ToWString(from), ToWString(to));
}




bool Utilities::String::Contains(const std::string& string, const std::string& substring, const bool& useCase)
{
    if (string.empty() || substring.empty())
        return false;

    if (useCase)
        return string.find(substring) != std::string::npos;

    std::string stringLower = ToLowerCase(string);
    std::string substringLower = ToLowerCase(substring);
    return stringLower.find(substringLower) != std::string::npos;
}

bool Utilities::String::Contains(const std::string& string, const std::wstring& substring, const bool& useCase)
{
    return Contains(string, ToString(substring), useCase);
}

bool Utilities::String::Contains(const char* cString, const char* substring, const bool& useCase)
{
    return Contains(ToString(cString), ToString(substring), useCase);
}


bool Utilities::String::Contains(const std::wstring& wString, const std::wstring& substring, const bool& useCase)
{
    if (wString.empty() || substring.empty())
        return false;

    if (useCase)
        return wString.find(substring) != std::wstring::npos;

    std::wstring wStringLower = ToLowerCase(wString);
    std::wstring substringLower = ToLowerCase(substring);
    return wStringLower.find(substringLower) != std::wstring::npos;
}

bool Utilities::String::Contains(const std::wstring& wString, const std::string& substring, const bool& useCase)
{
    return Contains(wString, ToWString(substring), useCase);
}

bool Utilities::String::Contains(const wchar_t* wcString, const wchar_t* substring, const bool& useCase)
{
    return Contains(ToWString(wcString), ToWString(substring), useCase);
}




bool Utilities::String::StartsWith(const std::string& string, const std::string& substring, const bool& useCase)
{
    if (substring.empty())
        return true;

    if (string.empty() || string.length() < substring.length())
        return false;

    if (useCase)
        return string.compare(0, substring.length(), substring) == 0;

    std::string stringLower = ToLowerCase(string);
    std::string substringLower = ToLowerCase(substring);
    return stringLower.compare(0, substringLower.length(), substringLower) == 0;
}

bool Utilities::String::StartsWith(const std::string& string, const std::wstring& substring, const bool& useCase)
{
    return StartsWith(string, ToString(substring), useCase);
}

bool Utilities::String::StartsWith(const char* cString, const char* substring, const bool& useCase)
{
    return StartsWith(ToString(cString), ToString(substring), useCase);
}


bool Utilities::String::StartsWith(const std::wstring& wString, const std::wstring& substring, const bool& useCase)
{
    if (substring.empty())
        return true;

    if (wString.empty() || wString.length() < substring.length())
        return false;

    if (useCase)
        return wString.compare(0, substring.length(), substring) == 0;

    std::wstring wStringLower = ToLowerCase(wString);
    std::wstring substringLower = ToLowerCase(substring);
    return wStringLower.compare(0, substringLower.length(), substringLower) == 0;
}

bool Utilities::String::StartsWith(const std::wstring& wString, const std::string& substring, const bool& useCase)
{
    return StartsWith(wString, ToWString(substring), useCase);
}

bool Utilities::String::StartsWith(const wchar_t* wcString, const wchar_t* substring, const bool& useCase)
{
    return StartsWith(ToWString(wcString), ToWString(substring), useCase);
}




bool Utilities::String::EndsWith(const std::string& string, const std::string& substring, const bool& useCase)
{
    if (substring.empty())
        return true;

    if (string.empty() || string.length() < substring.length())
        return false;

    if (useCase)
        return string.compare(string.length() - substring.length(), substring.length(), substring) == 0;

    std::string stringLower = ToLowerCase(string);
    std::string substringLower = ToLowerCase(substring);
    return stringLower.compare(stringLower.length() - substringLower.length(), substringLower.length(), substringLower) == 0;
}

bool Utilities::String::EndsWith(const std::string& string, const std::wstring& substring, const bool& useCase)
{
    return EndsWith(string, ToString(substring), useCase);
}

bool Utilities::String::EndsWith(const char* cString, const char* substring, const bool& useCase)
{
    return EndsWith(ToString(cString), ToString(substring), useCase);
}


bool Utilities::String::EndsWith(const std::wstring& wString, const std::wstring& substring, const bool& useCase)
{
    if (substring.empty())
        return true;

    if (wString.empty() || wString.length() < substring.length())
        return false;

    if (useCase)
        return wString.compare(wString.length() - substring.length(), substring.length(), substring) == 0;

    std::wstring wStringLower = ToLowerCase(wString);
    std::wstring substringLower = ToLowerCase(substring);
    return wStringLower.compare(wStringLower.length() - substringLower.length(), substringLower.length(), substringLower) == 0;
}

bool Utilities::String::EndsWith(const std::wstring& wString, const std::string& substring, const bool& useCase)
{
    return EndsWith(wString, ToWString(substring), useCase);
}

bool Utilities::String::EndsWith(const wchar_t* wcString, const wchar_t* substring, const bool& useCase)
{
    return EndsWith(ToWString(wcString), ToWString(substring), useCase);
}




std::vector<std::string> Utilities::String::Split(const std::string& string, const char& separator, const bool& removeSeparatorSpaces)
{
    std::vector<std::string> outCollection;

    if (string.empty())
        return outCollection;

    size_t position = 0;
    while (true)
    {
        const size_t separatorPosition = string.find(separator, position);

        std::string token = (separatorPosition == std::string::npos)
            ? string.substr(position)
            : string.substr(position, separatorPosition - position);

        if (removeSeparatorSpaces)
        {
            size_t first = 0;
            while (first < token.size() && std::isspace(token[first]))
                ++first;

            size_t last = token.size();
            while (last > first && std::isspace(token[last - 1]))
                --last;

            token = token.substr(first, last - first);
        }

        if (token.empty() == false)
            outCollection.push_back(token);

        if (separatorPosition == std::string::npos)
            break;

        position = separatorPosition + 1;
    }

    return outCollection;
}

std::vector<std::string> Utilities::String::Split(const std::wstring& wString, const char& separator, const bool& removeSeparatorSpaces)
{
    std::string string = ToString(wString);
    return Split(string, separator, removeSeparatorSpaces);
}

std::vector<std::string> Utilities::String::Split(const char* cString, const char& separator, const bool& removeSeparatorSpaces)
{
    std::string string = ToString(cString);
    return Split(string, separator, removeSeparatorSpaces);
}

std::vector<std::string> Utilities::String::Split(const wchar_t* wcString, const char& separator, const bool& removeSeparatorSpaces)
{
    std::string string = ToString(wcString);
    return Split(string, separator, removeSeparatorSpaces);
}


std::vector<std::wstring> Utilities::String::Split(const std::string& string, const wchar_t& separator, const bool& removeSeparatorSpaces)
{
    std::wstring wString = ToWString(string);
    return Split(wString, separator, removeSeparatorSpaces);
}

std::vector<std::wstring> Utilities::String::Split(const std::wstring& wideString, const wchar_t& separator, const bool& removeSeparatorSpaces)
{
    std::vector<std::wstring> outCollection;

    if (wideString.empty())
        return outCollection;

    size_t position = 0;
    while (true)
    {
        const size_t separatorPosition = wideString.find(separator, position);

        std::wstring token = (separatorPosition == std::wstring::npos)
            ? wideString.substr(position)
            : wideString.substr(position, separatorPosition - position);

        if (removeSeparatorSpaces)
        {
            size_t first = 0;
            while (first < token.size() && std::iswspace(token[first]))
                ++first;

            size_t last = token.size();
            while (last > first && std::iswspace(token[last - 1]))
                --last;

            token = token.substr(first, last - first);
        }

        if (token.empty() == false)
            outCollection.push_back(token);

        if (separatorPosition == std::wstring::npos)
            break;

        position = separatorPosition + 1;
    }

    return outCollection;
}

std::vector<std::wstring> Utilities::String::Split(const char* cString, const wchar_t& separator, const bool& removeSeparatorSpaces)
{
    std::wstring wString = ToWString(cString);
    return Split(wString, separator, removeSeparatorSpaces);
}

std::vector<std::wstring> Utilities::String::Split(const wchar_t* wcString, const wchar_t& separator, const bool& removeSeparatorSpaces)
{
    std::wstring wString = ToWString(wcString);
    return Split(wString, separator, removeSeparatorSpaces);
}




std::string Utilities::String::Reverse(std::string string)
{
    std::reverse(string.begin(), string.end());
    return string;
}

std::wstring Utilities::String::Reverse(std::wstring wString)
{
    std::reverse(wString.begin(), wString.end());
    return wString;
}

std::string Utilities::String::Reverse(const char* cString)
{
    return Reverse(ToString(cString));
}

std::wstring Utilities::String::Reverse(const wchar_t* wcString)
{
    return Reverse(ToWString(wcString));
}




Utilities::Game::E_GameState Utilities::Game::GetGameState()
{
    SDK::UWorld* world = SDK::UWorld::GetWorld();
    return GetGameState(world);
}

Utilities::Game::E_GameState Utilities::Game::GetGameState(SDK::UWorld* worldReference)
{
    if (worldReference == nullptr)
        return E_GameState::None;

    std::wstring levelName = SDK::UGameplayStatics::GetCurrentLevelName(worldReference, true).ToWString();
    if (levelName == L"StartScreen")
        return E_GameState::StartScreen;
    else if (levelName == L"OfflineLobby")
        return E_GameState::OfflineLobby;
    else if (levelName == L"OnlineLobby")
        return E_GameState::OnlineLobby;
    else if (String::Contains(levelName, L"Tutorial"))
        return E_GameState::Tutorial;
    else if (levelName == L"ProceduralLevel")
        return E_GameState::Match;
    else
        return E_GameState::None;
}


bool Utilities::Game::IsAtStartScreen()
{
    return GetGameState() == E_GameState::StartScreen;
}

bool Utilities::Game::IsAtStartScreen(SDK::UWorld* worldReference)
{
    return GetGameState(worldReference) == E_GameState::StartScreen;
}


bool Utilities::Game::IsInOfflineLobby()
{
    return GetGameState() == E_GameState::OfflineLobby;
}

bool Utilities::Game::IsInOfflineLobby(SDK::UWorld* worldReference)
{
    return GetGameState(worldReference) == E_GameState::OfflineLobby;
}


bool Utilities::Game::IsInOnlineLobby()
{
    return GetGameState() == E_GameState::OnlineLobby;
}

bool Utilities::Game::IsInOnlineLobby(SDK::UWorld* worldReference)
{
    return GetGameState(worldReference) == E_GameState::OnlineLobby;
}


bool Utilities::Game::IsInTutorial()
{
    return GetGameState() == E_GameState::Tutorial;
}

bool Utilities::Game::IsInTutorial(SDK::UWorld* worldReference)
{
    return GetGameState(worldReference) == E_GameState::Tutorial;
}


bool Utilities::Game::IsInMatch()
{
    return GetGameState() == E_GameState::Match;
}

bool Utilities::Game::IsInMatch(SDK::UWorld* worldReference)
{
    return GetGameState(worldReference) == E_GameState::Match;
}




bool Utilities::Game::IsInMenuOrLobby()
{
    E_GameState gameState = GetGameState();
    return (gameState == E_GameState::OfflineLobby || gameState == E_GameState::OnlineLobby);
}

bool Utilities::Game::IsInMenuOrLobby(SDK::UWorld* worldReference)
{
    E_GameState gameState = GetGameState(worldReference);
    return (gameState == E_GameState::OfflineLobby || gameState == E_GameState::OnlineLobby);
}


bool Utilities::Game::IsInGameplay()
{
    E_GameState gameState = GetGameState();
    return (gameState == E_GameState::Tutorial || gameState == E_GameState::Match);
}

bool Utilities::Game::IsInGameplay(SDK::UWorld* worldReference)
{
    E_GameState gameState = GetGameState(worldReference);
    return (gameState == E_GameState::Tutorial || gameState == E_GameState::Match);
}




bool Utilities::Game::IsInitialized()
{
    if (SDK::UObject::GObjects == nullptr || SDK::UObject::GObjects->Num() == 0)
        return false;

    SDK::UWorld* world = SDK::UWorld::GetWorld();
    if (world == nullptr)
        return false;

    return SDK::UGameplayStatics::GetGameInstance(world);
}




SDK::USplashScreen* Utilities::Game::GetSplashScreen()
{
    int32_t objectsNum = SDK::UObject::GObjects->Num();

    for (int i = 0; i < objectsNum; i++)
    {
        SDK::UObject* objectReference = SDK::UObject::GObjects->GetByIndex(i);

        if (objectReference == nullptr || objectReference->IsDefaultObject())
            continue;

        if (objectReference->IsA(SDK::USplashScreen::StaticClass()))
        {
            SDK::USplashScreen* splashScreenReference = static_cast<SDK::USplashScreen*>(objectReference);
            return splashScreenReference;
        }
    }

    return nullptr;
}





bool Utilities::Memory::Internal::IsValidPtr(const void* memoryPtr)
{
    MEMORY_BASIC_INFORMATION mbi;

    if (VirtualQuery(memoryPtr, &mbi, sizeof(mbi)) != sizeof(mbi)) // Try to request (query) information about the given memory region, return False if attempt fails.
        return false;

    if (mbi.State != MEM_COMMIT) // Memory region must be commited (actually backed by physical memory). Return False if it's not.
        return false;

    /* Strip out guard and cache flags to examine the core protection flags. */
    DWORD protectionFlags = mbi.Protect & ~(PAGE_GUARD | PAGE_NOCACHE | PAGE_WRITECOMBINE);
    if ((protectionFlags & (PAGE_READONLY | PAGE_READWRITE | PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE)) == false) // See if we're able to request one of the read permissions (readable or executable+read), return False if we aren't.
        return false;

    return true;
}

bool Utilities::Memory::Internal::IsValidAddress(const uintptr_t& memoryAddress)
{
    void* memoryPtr = reinterpret_cast<void*>(memoryAddress);
    return IsValidPtr(memoryPtr);
}




void* Utilities::Memory::Internal::PtrAddOffset(const void* memoryPtr, size_t offset)
{
    /* Since void* doesn't support pointer arithmetics, we need to convert it to uintptr_t first. */
    uintptr_t memoryAddress = reinterpret_cast<uintptr_t>(memoryPtr);
    return PtrAddOffset(memoryAddress, offset);
}

void* Utilities::Memory::Internal::PtrAddOffset(const uintptr_t& memoryAddress, size_t offset)
{
    /* Calculate the new address by adding the offset. */
    uintptr_t newMemoryAddress = AddressAddOffset(memoryAddress, offset);
    if (newMemoryAddress == 0x0) // Return null pointer if memory region is invalid.
        return nullptr;

    return reinterpret_cast<void*>(newMemoryAddress);
}


uintptr_t Utilities::Memory::Internal::AddressAddOffset(const void* memoryPtr, size_t offset)
{
    /* Since void* doesn't support pointer arithmetics, we need to convert it to uintptr_t first. */
    uintptr_t memoryAddress = reinterpret_cast<uintptr_t>(memoryPtr);
    return AddressAddOffset(memoryAddress, offset);
}

uintptr_t Utilities::Memory::Internal::AddressAddOffset(const uintptr_t& memoryAddress, size_t offset)
{
    /* Calculate the new address by adding the offset. */
    uintptr_t newMemoryAddress = memoryAddress + offset;

    /* Before dereferencing, verify we can read sizeof(uintptr_t) bytes at 'newMemoryAddress'. */
    /* Abort and return 0 if memory isn't readable */
    if (IsValidAddress(newMemoryAddress) == false ||
        IsValidAddress((newMemoryAddress + sizeof(uintptr_t) - 1)) == false)
        return 0x0;

    /* Return the valid address */
    return newMemoryAddress;
}




void* Utilities::Memory::Internal::PtrFollowPointerChain(const void* memoryPtr, const std::vector<uintptr_t>& memoryOffsets)
{
    /* Since void* doesn't support pointer arithmetics, we need to convert it to uintptr_t first. */
    uintptr_t memoryAddress = reinterpret_cast<uintptr_t>(memoryPtr);

    /* Calculate the new address by following the pointer chain. */
    uintptr_t newMemoryAddress = AddressFollowPointerChain(memoryAddress, memoryOffsets);
    if (newMemoryAddress == 0x0) // Return null pointer if memory region is invalid.
        return nullptr;

    return reinterpret_cast<void*>(newMemoryAddress);
}

uintptr_t Utilities::Memory::Internal::AddressFollowPointerChain(const uintptr_t& memoryAddress, const std::vector<uintptr_t>& memoryOffsets)
{
    uintptr_t newMemoryAddress = memoryAddress;
    size_t offsetsCount = memoryOffsets.size();

    for (size_t i = 0; i < offsetsCount; ++i)
    {
        /* Before dereferencing, verify we can read sizeof(uintptr_t) bytes at 'newMemoryAddress'. */
        /* Abort and return 0 if memory isn't readable */
        if (IsValidAddress(newMemoryAddress) == false ||
            IsValidAddress((newMemoryAddress + sizeof(uintptr_t) - 1)) == false)
            return 0x0;

        /* Read the next pointer value from memory and add the current offset to advance to the next address in the chain. */
        newMemoryAddress = *reinterpret_cast<uintptr_t*>(newMemoryAddress);
        newMemoryAddress += memoryOffsets[i];
    }

    /* After processing all offsets, 'ptr' should point to the final data. */
    return newMemoryAddress;
}




std::wstring Utilities::Memory::Internal::GetWString(const void* memoryPtr)
{
    uintptr_t memoryAddress = reinterpret_cast<uintptr_t>(memoryPtr);
    return GetWString(memoryAddress);
}

std::wstring Utilities::Memory::Internal::GetWString(const void* memoryPtr, size_t maxLength)
{
    uintptr_t memoryAddress = reinterpret_cast<uintptr_t>(memoryPtr);
    return GetWString(memoryAddress, maxLength);
}

std::wstring Utilities::Memory::Internal::GetWString(const uintptr_t& memoryAddress)
{
    /* Verify that the address is valid. */
    if (IsValidAddress(memoryAddress) == false)
        return std::wstring();

    /* Read and return the wide string from the target address. */
    const wchar_t* wStrPtr = reinterpret_cast<const wchar_t*>(memoryAddress);
    return std::wstring(wStrPtr);
}

std::wstring Utilities::Memory::Internal::GetWString(const uintptr_t& memoryAddress, size_t maxLength)
{
    /* Verify that the address is valid. */
    if (IsValidAddress(memoryAddress) == false)
        return std::wstring();

    /* Read and return the wide string from the target address. */
    const wchar_t* wStrPtr = reinterpret_cast<const wchar_t*>(memoryAddress);
    return std::wstring(wStrPtr, wcsnlen_s(wStrPtr, maxLength));
}

bool Utilities::Memory::Internal::SetWString(const void* memoryPtr, const std::wstring& newValue)
{
    uintptr_t memoryAddress = reinterpret_cast<uintptr_t>(memoryPtr);
    return SetWString(memoryAddress, newValue);
}

bool Utilities::Memory::Internal::SetWString(const uintptr_t& memoryAddress, const std::wstring& newValue)
{
    /* Verify that the address is valid. */
    if (IsValidAddress(memoryAddress) == false)
        return false;

    /* Cast the address to wchar_t* */
    wchar_t* targetStr = reinterpret_cast<wchar_t*>(memoryAddress);
    size_t byteSize = (newValue.size() + 1) * sizeof(wchar_t); // +1 for null terminator

    /* Make the memory region writable. */
    DWORD oldProtect;
    if (VirtualProtect(targetStr, byteSize, PAGE_EXECUTE_READWRITE, &oldProtect) == false)
        return false;

    /* Write the new string value. */
    memcpy(targetStr, newValue.c_str(), byteSize);

    /* Restore the original protection. */
    DWORD tmp;
    VirtualProtect(targetStr, byteSize, oldProtect, &tmp);

    return true;
}

bool Utilities::Memory::Internal::PatchWString(const void* memoryPtr, const std::wstring& from, const std::wstring& to)
{
    uintptr_t memoryAddress = reinterpret_cast<uintptr_t>(memoryPtr);
    return PatchWString(memoryAddress, from, to);
}

bool Utilities::Memory::Internal::PatchWString(const uintptr_t& memoryAddress, const std::wstring& from, const std::wstring& to)
{
    /* Verify that the address is valid. */
    if (IsValidAddress(memoryAddress) == false)
        return false;

    /* Verify that the current value matches the expected one. */
    const wchar_t* currentStr = reinterpret_cast<const wchar_t*>(memoryAddress);
    if (wcsncmp(currentStr, from.c_str(), from.size()) != 0)
        return false;

    size_t byteSize = (to.size() + 1) * sizeof(wchar_t);

    /* Make the memory region writable */
    DWORD oldProtect;
    if (VirtualProtect(reinterpret_cast<void*>(memoryAddress), byteSize, PAGE_EXECUTE_READWRITE, &oldProtect) == false)
        return false;

    /* Write the new string value */
    memcpy(reinterpret_cast<void*>(memoryAddress), to.c_str(), byteSize);

    /* Restore the original protection */
    DWORD tmp;
    VirtualProtect(reinterpret_cast<void*>(memoryAddress), byteSize, oldProtect, &tmp);

    return true;
}