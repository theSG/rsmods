#pragma once
#include "windows.h"
#include <vector>

namespace MemUtil {
	bool bCompare(const BYTE* pData, const byte* bMask, const char* szMask);
	bool PatchAdr(LPVOID dst, LPVOID src, size_t len);
	bool PatchAdr(uintptr_t address, LPVOID changeToMake, size_t len, bool addBaseHandle);
	bool PlaceHook(void* hookSpot, void* ourFunct, int len);
	PBYTE TrampHook(PBYTE src, PBYTE dst, unsigned int len);
	bool IsBadReadPtr(void* p);
	uintptr_t FindDMAAddy(uintptr_t ptr, std::vector<unsigned int> offsets, bool safe = false);
	uint8_t* FindPatternPtr(uint32_t dwAddress, size_t dwLen, uint8_t* pattern, char* mask);
	uintptr_t ReadPtr(uintptr_t adr);
	template <typename T>
	bool SetStaticValue(uintptr_t staticValue, T data, unsigned int lengthOfData);

	template <typename T>
	T FindPattern(uint32_t address, size_t size, PBYTE pattern, char* mask);

	template <typename T>
	T ReadValue(uintptr_t adr, bool addBaseHandle);
};

template <typename T>
T MemUtil::FindPattern(uint32_t address, size_t size, PBYTE pattern, char* mask) {
	for (uint32_t i = 0; i < size; i++)
		if (bCompare((PBYTE)(address + i), pattern, mask))
			return (T)(address + i);

	return NULL;
}

template <typename T>
/// <summary>
/// Sets a static value in the executable, utilizing VirtualProtect.
/// </summary>
/// <typeparam name="T"> - Type of the data.</typeparam>
/// <param name="staticValue"> - Address of the static value.</param>
/// <param name="data"> - Data we should change the staticValue to.</param>
/// <param name="lengthOfData"> - Length of the data (needed for VirtualProtect).</param>
/// <returns>Successfully able to set the value.</returns>
bool MemUtil::SetStaticValue(uintptr_t staticValue, T data, unsigned int lengthOfData) {
	DWORD dwOldProt, dwDummy;

	// Save old Virtual Protect status, but allow us to Execute, Read, and Write to the executable's memory so we can make this change.
	if (!VirtualProtect((LPVOID)staticValue, lengthOfData, PAGE_EXECUTE_READWRITE, &dwOldProt)) {
		return false;
	}

	// Write what we had in data to the staticValue.
	*(T*)staticValue = data;

	// Resets the virtual protect to the status we saved earlier in this function. 
	VirtualProtect((LPVOID)staticValue, lengthOfData, dwOldProt, &dwDummy);

	return true;
}

template <typename T>
T MemUtil::ReadValue(uintptr_t address, bool addBaseHandle) {
	if (address == NULL)
		return NULL;
	uintptr_t addr = address + (addBaseHandle ? Offsets::baseHandle : 0);
	return *(T*)addr;
}
