// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "Trampoline.h"
#include "umvc3utils.h"
#include "MemoryMgr.h"
#include "IniReader.h"
using namespace Memory::VP;

int toggleHudKey;

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

struct sBattleSetting {
    char pad[0x370];
    std::byte battle_ui_disp;
    std::byte input_key_disp;
    std::byte finish_picture_save;
    std::byte replay_use;
    std::byte damage_disp;
};

static int64 timer = GetTickCount64();

void keyHandle() {
    if (GetAsyncKeyState(toggleHudKey)) {
        if (GetTickCount64() - timer <= 150) return;
        timer = GetTickCount64();
        sBattleSetting* battle_setup = *reinterpret_cast<sBattleSetting**>(_addr(0x140d50e58));
        if (battle_setup != nullptr) {
            battle_setup->battle_ui_disp = battle_setup->battle_ui_disp ^ (std::byte)0x1;
        }
    }
}

void WINAPI HookUpdate()
{
    while (true)
    {
        keyHandle();
        Sleep(1);
    }
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
           CIniReader ini;

           toggleHudKey = ini.ReadInteger("Settings", "toggleHudKey", VK_SPACE);

            CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)HookUpdate, hModule, 0, nullptr);
        }
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

