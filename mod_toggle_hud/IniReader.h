#pragma once
class CIniReader
{
public:
	CIniReader();
	int ReadInteger(const char* szSection, const char* szKey, int iDefaultValue);
private:
	char m_szFileName[255];
};