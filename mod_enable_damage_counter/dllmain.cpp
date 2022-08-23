// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "Trampoline.h"
#include "umvc3utils.h"
#include "MemoryMgr.h"
using namespace Memory::VP;

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

struct BattleSetup {
    char pad[0x370];
    std::byte battle_ui_disp;
    std::byte input_key_disp;
    std::byte finish_picture_save;
    std::byte replay_use;
    std::byte damage_disp;
};

BattleSetup* HookBattleSetup(BattleSetup* param)
{
    BattleSetup * battle_setup = ((BattleSetup*(__fastcall*)(BattleSetup*))_addr(0x14024af90))(param);
    battle_setup->damage_disp = (std::byte)0x1;
    return battle_setup;
}

void OnInitializeHook()
{

    Trampoline* tramp = Trampoline::MakeTrampoline(GetModuleHandle(nullptr));
    InjectHook(_addr(0x14024d72f), tramp->Jump(HookBattleSetup), PATCH_JUMP);
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        if (CheckGame()) {
            OnInitializeHook();
        }
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

