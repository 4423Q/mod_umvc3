#include "pch.h"
#include "umvc3utils.h"

int64 GetUMvC3EntryPoint()
{
	static __int64 addr = reinterpret_cast<__int64>(GetModuleHandle(nullptr));
	return addr;
}

int64 _addr(__int64 addr)
{
	return GetUMvC3EntryPoint() - 0x140000000 + addr;
}