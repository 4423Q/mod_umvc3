// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "MemoryMgr.h"
#include "umvc3utils.h"
#include "includes.h"
#include "InputDisplay.h"
#include <d3d9.h>
#include "Trampoline.h"
#pragma comment(lib, "d3dx9.lib")
using namespace Memory::VP;


struct sBattleSetting {
    void* vtable;
    char pad[0x30];
    struct character {
        void* vtable;
        INT32 mTeam;
        INT32 mType;
        INT32 unknown;
        INT32 mBody;
        INT32 mCpu;
        INT32 assist;
        char pad[0x38];
    };
    character characters[6];
    char pad2[0x104];
    INT32 battle_type;
    char pad3[0x21];
    char input_key_disp;
};

struct sRender {
    char pad[0xd8];
    int screen_width;
    int screen_height;
};

EndScene oEndScene = NULL;

WNDPROC oWndProc;
static HWND window = NULL;
HMODULE module = NULL;

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



void HookInput(void* whatever, int input)
{
    InputDisplay::sendInput(input);
    return;
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
        sRender* renderInfo = *reinterpret_cast<sRender**>(_addr(0x140e179a8));
        InputDisplay::init(pDevice, renderInfo->screen_width);
        Trampoline* tramp = Trampoline::MakeTrampoline(GetModuleHandle(nullptr));
        InjectHook(_addr(0x1402f9c36), tramp->Jump(HookInput), PATCH_CALL);
        init = true;
    }

    InputDisplay::drawFrame();

    sBattleSetting* battleSetting = *reinterpret_cast<sBattleSetting**>(_addr(0x140d50e58));
    if (battleSetting->battle_type != 5 || battleSetting->input_key_disp == 0) {
        InputDisplay::setActive(false);
    }

    return oEndScene(pDevice);
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
            module = hModule;
            DisableThreadLibraryCalls(hModule);
            CreateThread(nullptr, 0, MainThread, hModule, 0, nullptr);
        }
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}
