// f2r.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
// f2r.cpp : Writes info from a file to the registry (HKEY_CURRENT_USER), using Win32 API
// usage: f2r <filename> <keyname> <valuename>
// Result: the bytes from the file are written to the value as REG_BINARY
//

#include <iostream>
#include <Windows.h>
#include <string>
#include <vector>

long GetDataFromFile(HANDLE h, std::vector<BYTE>& val)
{
	DWORD dataSize = 0;
	dataSize = GetFileSize(h, NULL);
	if (dataSize == INVALID_FILE_SIZE)
	{
		return GetLastError();
	}
	try
	{
		val.clear();
		val.resize(dataSize);
		DWORD nb;
		if (!ReadFile(h, val.data(), dataSize, &nb, NULL))
		{
			return GetLastError();
		}
		return ERROR_SUCCESS;
	}
	catch (const std::exception&)
	{
		return ERROR_OUTOFMEMORY;
	}
}

int main(int ac, char* av[])
{
	using RegHandlePtr = std::unique_ptr<HKEY__, decltype(&RegCloseKey)>;
	using FileHandlePtr = std::unique_ptr<void, decltype(&CloseHandle)>;

	if (ac < 4)
	{
		std::cout << "Invalid arguments" << "n";
		return 0;
	}
	std::string fileName = av[1];
	std::string keyName = av[2];
	std::string valName = av[3];

	HANDLE hFile = CreateFile(fileName.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		DWORD dwErr = GetLastError();
		std::cout << "r2f: CreateFile failed: " << dwErr << "n";
		return 0;
	}
	FileHandlePtr fh(hFile, CloseHandle);

	std::vector<BYTE> data;
	LONG n = GetDataFromFile(hFile, data);
	if (n != ERROR_SUCCESS)
	{
		std::cout << "r2f: GetDataFromFile failed: " << n << "n";
		return 0;
	}

	HKEY hKey;
	n = RegCreateKeyEx(HKEY_CURRENT_USER, keyName.c_str(), 0, NULL, 0, KEY_WRITE, NULL, &hKey, NULL);
	if (n != ERROR_SUCCESS)
	{
		std::cout << "r2f: RegOpenKey failed: " << n << "n";
		return 0;
	}
	RegHandlePtr rh(hKey, RegCloseKey);

	n = RegSetValueEx(hKey, valName.c_str(), 0, REG_BINARY, data.data(), data.size());
	return 0;
}
