#pragma once
#include <Windows.h>
#include <d3d9.h>
#include "kiero/kiero.h"
#include "kiero/minhook/include/MinHook.h"

typedef long(__stdcall* EndScene)(LPDIRECT3DDEVICE9);
typedef LRESULT(CALLBACK* WNDPROC)(HWND, UINT, WPARAM, LPARAM);