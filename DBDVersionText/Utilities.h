#pragma once
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>
#include <cwctype>
#include <cstdlib>
#include <cstdint>
#include <utility>

#include "Windows.h"
#include "SDK.hpp"

#pragma comment(lib, "shell32.lib")






namespace Utilities
{
	class CommandLine
	{
	public:
		static std::string Get();
		static std::wstring GetUtf16();

		static std::vector<std::string> GetArguments();
		static std::vector<std::wstring> GetArgumentsUtf16();

		static int Count();

		static bool HasArgument(const std::string& argument);
		static bool HasArgument(const std::wstring& argument);
		static bool HasArgument(const char* argument);
		static bool HasArgument(const wchar_t* argument);
	};




	class String
	{
	public:
		static std::string ToString(const std::wstring& wideString);
		static std::string ToString(const char* cString);
		static std::string ToString(const wchar_t* wcString);

		static std::wstring ToWString(const std::string& string);
		static std::wstring ToWString(const char* cString);
		static std::wstring ToWString(const wchar_t* wcString);


		static std::string ToLowerCase(std::string string);
		static std::wstring ToLowerCase(std::wstring wString);
		static std::string ToLowerCase(const char* cString);
		static std::wstring ToLowerCase(const wchar_t* wcString);

		static std::string ToUpperCase(std::string string);
		static std::wstring ToUpperCase(std::wstring wString);
		static std::string ToUpperCase(const char* cString);
		static std::wstring ToUpperCase(const wchar_t* wcString);


		static std::string Replace(std::string string, const std::string& from, const std::string& to);
		static std::string Replace(const std::string& string, const std::string& from, const std::wstring& to);
		static std::string Replace(const std::string& string, const std::wstring& from, const std::string& to);
		static std::string Replace(const std::string& string, const std::wstring& from, const std::wstring& to);
		static std::string Replace(const char* cString, const char* from, const char* to);

		static std::wstring Replace(std::wstring wString, const std::wstring& from, const std::wstring& to);
		static std::wstring Replace(const std::wstring& wString, const std::wstring& from, const std::string& to);
		static std::wstring Replace(const std::wstring& wString, const std::string& from, const std::wstring& to);
		static std::wstring Replace(const std::wstring& wString, const std::string& from, const std::string& to);
		static std::wstring Replace(const wchar_t* wcString, const wchar_t* from, const wchar_t* to);


		static std::string ReplaceFirstOf(std::string string, const std::string& from, const std::string& to);
		static std::string ReplaceFirstOf(const std::string& string, const std::string& from, const std::wstring& to);
		static std::string ReplaceFirstOf(const std::string& string, const std::wstring& from, const std::string& to);
		static std::string ReplaceFirstOf(const std::string& string, const std::wstring& from, const std::wstring& to);
		static std::string ReplaceFirstOf(const char* cString, const char* from, const char* to);

		static std::wstring ReplaceFirstOf(std::wstring wString, const std::wstring& from, const std::wstring& to);
		static std::wstring ReplaceFirstOf(const std::wstring& wString, const std::wstring& from, const std::string& to);
		static std::wstring ReplaceFirstOf(const std::wstring& wString, const std::string& from, const std::wstring& to);
		static std::wstring ReplaceFirstOf(const std::wstring& wString, const std::string& from, const std::string& to);
		static std::wstring ReplaceFirstOf(const wchar_t* wcString, const wchar_t* from, const wchar_t* to);


		static std::string ReplaceLastOf(std::string string, const std::string& from, const std::string& to);
		static std::string ReplaceLastOf(const std::string& string, const std::string& from, const std::wstring& to);
		static std::string ReplaceLastOf(const std::string& string, const std::wstring& from, const std::string& to);
		static std::string ReplaceLastOf(const std::string& string, const std::wstring& from, const std::wstring& to);
		static std::string ReplaceLastOf(const char* cString, const char* from, const char* to);

		static std::wstring ReplaceLastOf(std::wstring wString, const std::wstring& from, const std::wstring& to);
		static std::wstring ReplaceLastOf(const std::wstring& wString, const std::wstring& from, const std::string& to);
		static std::wstring ReplaceLastOf(const std::wstring& wString, const std::string& from, const std::wstring& to);
		static std::wstring ReplaceLastOf(const std::wstring& wString, const std::string& from, const std::string& to);
		static std::wstring ReplaceLastOf(const wchar_t* wcString, const wchar_t* from, const wchar_t* to);


		static bool Contains(const std::string& string, const std::string& substring, const bool& useCase = true);
		static bool Contains(const std::string& string, const std::wstring& substring, const bool& useCase = true);
		static bool Contains(const char* cString, const char* substring, const bool& useCase = true);

		static bool Contains(const std::wstring& wString, const std::wstring& substring, const bool& useCase = true);
		static bool Contains(const std::wstring& wString, const std::string& substring, const bool& useCase = true);
		static bool Contains(const wchar_t* wcString, const wchar_t* substring, const bool& useCase = true);


		static bool StartsWith(const std::string& string, const std::string& substring, const bool& useCase = true);
		static bool StartsWith(const std::string& string, const std::wstring& substring, const bool& useCase = true);
		static bool StartsWith(const char* cString, const char* substring, const bool& useCase = true);

		static bool StartsWith(const std::wstring& wString, const std::wstring& substring, const bool& useCase = true);
		static bool StartsWith(const std::wstring& wString, const std::string& substring, const bool& useCase = true);
		static bool StartsWith(const wchar_t* wcString, const wchar_t* substring, const bool& useCase = true);


		static bool EndsWith(const std::string& string, const std::string& substring, const bool& useCase = true);
		static bool EndsWith(const std::string& string, const std::wstring& substring, const bool& useCase = true);
		static bool EndsWith(const char* cString, const char* substring, const bool& useCase = true);

		static bool EndsWith(const std::wstring& wString, const std::wstring& substring, const bool& useCase = true);
		static bool EndsWith(const std::wstring& wString, const std::string& substring, const bool& useCase = true);
		static bool EndsWith(const wchar_t* wcString, const wchar_t* substring, const bool& useCase = true);


		static std::vector<std::string> Split(const std::string& string, const char& separator, const bool& removeSeparatorSpaces = true);
		static std::vector<std::string> Split(const std::wstring& wString, const char& separator, const bool& removeSeparatorSpaces = true);
		static std::vector<std::string> Split(const char* cString, const char& separator, const bool& removeSeparatorSpaces = true);
		static std::vector<std::string> Split(const wchar_t* wcString, const char& separator, const bool& removeSeparatorSpaces = true);

		static std::vector<std::wstring> Split(const std::string& string, const wchar_t& separator, const bool& removeSeparatorSpaces = true);
		static std::vector<std::wstring> Split(const std::wstring& wideString, const wchar_t& separator, const bool& removeSeparatorSpaces = true);
		static std::vector<std::wstring> Split(const char* cString, const wchar_t& separator, const bool& removeSeparatorSpaces = true);
		static std::vector<std::wstring> Split(const wchar_t* wcString, const wchar_t& separator, const bool& removeSeparatorSpaces = true);


		static std::string Reverse(std::string string);
		static std::wstring Reverse(std::wstring wString);
		static std::string Reverse(const char* cString);
		static std::wstring Reverse(const wchar_t* wcString);
	};




	class Game
	{
	public:
		enum class E_GameState
		{
			None,
			StartScreen,
			OfflineLobby,
			OnlineLobby,
			Tutorial,
			Match
		};


		static E_GameState GetGameState();
		static E_GameState GetGameState(SDK::UWorld* worldReference);

		static bool IsAtStartScreen();
		static bool IsAtStartScreen(SDK::UWorld* worldReference);

		static bool IsInOfflineLobby();
		static bool IsInOfflineLobby(SDK::UWorld* worldReference);

		static bool IsInOnlineLobby();
		static bool IsInOnlineLobby(SDK::UWorld* worldReference);

		static bool IsInTutorial();
		static bool IsInTutorial(SDK::UWorld* worldReference);

		static bool IsInMatch();
		static bool IsInMatch(SDK::UWorld* worldReference);


		static bool IsInMenuOrLobby();
		static bool IsInMenuOrLobby(SDK::UWorld* worldReference);

		static bool IsInGameplay();
		static bool IsInGameplay(SDK::UWorld* worldReference);


		static bool IsInitialized();


		static SDK::USplashScreen* GetSplashScreen();
	};




	namespace Memory
	{
		class Internal // Functions within the class allows to manipulate memory of process program is running in.
		{
		public:
			/**
			* @brief Determines whether a given pointer refers to a valid, committed, and readable (or executable) memory region.
			* @param memoryPtr - Pointer to the memory location to check.
			* @return true if the region is committed, and has read or execute+read permissions; false otherwise.
			*/
			static bool IsValidPtr(const void* memoryPtr);
			/**
			* @brief Determines whether a given memory address refers to a valid, committed, and readable (or executable) memory region.
			* @param memoryAddress - Address of the memory location to check.
			* @return true if the region is committed, and has read or execute+read permissions; false otherwise.
			*/
			static bool IsValidAddress(const uintptr_t& memoryAddress);

			/**
			* @brief Adds a byte offset to a base memory address and verifies that the resulting address range is readable.
			* @param memoryPtr - Pointer to the starting memory location to which the offset will be added.
			* @param offset - The number of bytes to add to the base address.
			* @return The new valid pointer to the memory location if both the beginning and end of the target region are readable;
			*         otherwise returns 'nullptr' to indicate an invalid or unreadable address.
			*/
			static void* PtrAddOffset(const void* memoryPtr, size_t offset);
			/**
			* @brief Adds a byte offset to a base memory address and verifies that the resulting address range is readable.
			* @param memoryAddress - Address of the memory location to which the offset will be added.
			* @param offset - The number of bytes to add to the base address.
			* @return The new valid pointer to the memory location if both the beginning and end of the target region are readable;
			*         otherwise returns 'nullptr' to indicate an invalid or unreadable address.
			*/
			static void* PtrAddOffset(const uintptr_t& memoryAddress, size_t offset);

			/**
			* @brief Adds a byte offset to a base memory address and verifies that the resulting address range is readable.
			* @param memoryPtr - Pointer to the starting memory location to which the offset will be added.
			* @param offset - The number of bytes to add to the base address.
			* @return The new valid memory address if both the beginning and end of the target region are readable;
			*         otherwise returns '0x0' to indicate an invalid or unreadable address.
			*/
			static uintptr_t AddressAddOffset(const void* memoryPtr, size_t offset);
			/**
			* @brief Adds a byte offset to a base memory address and verifies that the resulting address range is readable.
			* @param memoryAddress - Address of the memory location to which the offset will be added.
			* @param offset - The number of bytes to add to the base address.
			* @return The new valid memory address if both the beginning and end of the target region are readable;
			*         otherwise returns '0x0' to indicate an invalid or unreadable address.
			*/
			static uintptr_t AddressAddOffset(const uintptr_t& memoryAddress, size_t offset);

			/**
			* @brief Follows a chain of pointers starting from a base address, applying a sequence of offsets.
			* @param memoryPtr - Pointer to the starting memory location from which the pointer chain will be resolved.
			* @return The new valid memory address if pointer chain was successfully resolved and final destination was reached;
			*         otherwise returns 'nullptr' to indicate an invalid or unreadable address.
			*/
			static void* PtrFollowPointerChain(const void* memoryPtr, const std::vector<uintptr_t>& memoryOffsets);
			/**
			* @brief Follows a chain of pointers starting from a base address, applying a sequence of offsets.
			* @param memoryAddress - Address of the memory location from which the pointer chain will be resolved.
			* @return The new valid memory address if pointer chain was successfully resolved and final destination was reached;
			*         otherwise returns '0x0' to indicate an invalid or unreadable address.
			*/
			static uintptr_t AddressFollowPointerChain(const uintptr_t& memoryAddress, const std::vector<uintptr_t>& memoryOffsets);




			/* For when 'memoryAddress' contains the actual value. */
			static std::wstring GetWString(const void* memoryPtr);
			static std::wstring GetWString(const void* memoryPtr, size_t maxLength);
			static std::wstring GetWString(const uintptr_t& memoryAddress);
			static std::wstring GetWString(const uintptr_t& memoryAddress, size_t maxLength);

			static bool			SetWString(const void* memoryPtr, const std::wstring& newValue);
			static bool			SetWString(const uintptr_t& memoryAddress, const std::wstring& newValue);

			static bool			PatchWString(const void* memoryPtr, const std::wstring& from, const std::wstring& to);
			static bool			PatchWString(const uintptr_t& memoryAddress, const std::wstring& from, const std::wstring& to);
		};
	}
};

