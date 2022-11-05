#include "pch.h"
#include "IniReader.h"
#include "framework.h"
#include <iostream>
#include <Windows.h>
using namespace std;
#pragma warning(disable:4996)
EXTERN_C IMAGE_DOS_HEADER __ImageBase;

CIniReader::CIniReader()
{
	char			moduleName[MAX_PATH];
	char			dllPath[MAX_PATH];
	char			iniName[MAX_PATH];
	char* tempPointer;

	GetModuleFileNameA((HINSTANCE)&__ImageBase, moduleName, MAX_PATH);
	tempPointer = strrchr(moduleName, '.');
	*tempPointer = '\0';
	tempPointer = strrchr(moduleName, '\\');
	strncpy(iniName, tempPointer + 1, 255);
	strcat(iniName, ".ini");
	strncpy(dllPath, moduleName, (tempPointer - moduleName + 1));
	dllPath[tempPointer - moduleName + 1] = '\0';

	strcat(dllPath, iniName);

	memset(m_szFileName, 0x00, 255);
	memcpy(m_szFileName, dllPath, strlen(dllPath));
}


int CIniReader::ReadInteger(const char* szSection, const char* szKey, int iDefaultValue)
{
	int iResult = GetPrivateProfileIntA(szSection, szKey, iDefaultValue, m_szFileName);
	return iResult;
}