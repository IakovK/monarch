// r2f.cpp : Writes info from registry (HKEY_CURRENT_USER) to a file, using Win32 API
// usage: r2f <keyname> <valuename> <filename>
// Result: the bytes from the value are written to the file
//

#include <iostream>
#include <Windows.h>
#include <string>
#include <vector>

long GetRegValue(HKEY hKey, std::string valName, std::vector<BYTE>& val)
{
	DWORD dataSize = 0;
	long n = RegQueryValueEx(hKey, valName.c_str(), NULL, NULL, NULL, &dataSize);
	if (n != ERROR_SUCCESS)
	{
		return n;
	}
	try
	{
		val.clear();
		val.resize(dataSize);
		n = RegQueryValueEx(hKey, valName.c_str(), NULL, NULL, val.data(), &dataSize);
		return n;
	}
	catch (const std::exception&)
	{
		return ERROR_OUTOFMEMORY;
	}
}

int main(int ac, char *av[])
{
	using RegHandlePtr = std::unique_ptr<HKEY__, decltype(&RegCloseKey)>;
	using FileHandlePtr = std::unique_ptr<void, decltype(&CloseHandle)>;

	if (ac < 4)
	{
		std::cout << "Invalid arguments" << "n";
		return 0;
	}
	std::string keyName = av[1];
	std::string valName = av[2];
	std::string fileName = av[3];

	HKEY hKey;
	LONG n = RegOpenKeyEx(HKEY_CURRENT_USER, keyName.c_str(), 0, KEY_READ, &hKey);
	if (n != ERROR_SUCCESS)
	{
		std::cout << "r2f: RegOpenKey failed: " << n << "n";
		return 0;
	}
	RegHandlePtr rh(hKey, RegCloseKey);

	std::vector<BYTE> data;
	n = GetRegValue(hKey, valName, data);
	if (n != ERROR_SUCCESS)
	{
		std::cout << "r2f: GetRegValue failed: " << n << "n";
		return 0;
	}

	HANDLE hFile = CreateFile(fileName.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		DWORD dwErr = GetLastError();
		std::cout << "r2f: CreateFile failed: " << dwErr << "n";
		return 0;
	}
	FileHandlePtr fh(hFile, CloseHandle);

	DWORD nb;
	WriteFile(hFile, data.data(), data.size(), &nb, NULL);
	return 0;
}
