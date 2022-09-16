// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "MemoryMgr.h"
#include "umvc3utils.h"
#include "includes.h"
#include "InputDisplay.h"
#include <d3d9.h>
#pragma comment(lib, "d3dx9.lib")
using namespace Memory::VP;


EndScene oEndScene = NULL;

WNDPROC oWndProc;
static HWND window = NULL;

bool CheckGame()
{
    char* gameName = (char*)(_addr(0x140B12D10));

    if (strcmp(gameName, "umvc3") == 0)
    {
        return true;
    }
    else
    {
        MessageBoxA(0, "Invalid game version!\nmod_enable_damage_counter only supports latest Steam executable.", 0, MB_ICONINFORMATION);
        return false;
    }

}



LRESULT __stdcall WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

    switch (uMsg)
    {
    case WM_KILLFOCUS:
        break;
    case WM_SETFOCUS:
        break;
        break;
    default:
        break;
    }

    return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
}




bool init = false;
long __stdcall hkEndScene(LPDIRECT3DDEVICE9 pDevice)
{
    if (!init)
    {
        D3DDEVICE_CREATION_PARAMETERS params;
        pDevice->GetCreationParameters(&params);
        window = params.hFocusWindow;
        oWndProc = (WNDPROC)SetWindowLongPtr(window, GWLP_WNDPROC, (LONG_PTR)WndProc);
        InputDisplay::init(pDevice);
        init = true;
    }

    InputDisplay::drawFrame();

    return oEndScene(pDevice);
}


void WINAPI HookUpdate()
{
    //InputDisplay::run();
}


DWORD WINAPI MainThread(LPVOID lpReserved)
{
    Sleep(5000);
    bool attached = false;
    do
    {
        if (kiero::init(kiero::RenderType::D3D9) == kiero::Status::Success)
        {
            kiero::bind(42, (void**)&oEndScene, hkEndScene);
            attached = true;
        }
    } while (!attached);
    return TRUE;
}



BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        if (CheckGame()) {
            DisableThreadLibraryCalls(hModule);
            CreateThread(nullptr, 0, MainThread, hModule, 0, nullptr);
            CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)HookUpdate, hModule, 0, nullptr);
        }
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}
