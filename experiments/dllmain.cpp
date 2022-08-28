// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "Trampoline.h"
#include "umvc3utils.h"
#include "MemoryMgr.h"
#include "umvc3_sMvc3Main.h"
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
    BattleSetup* battle_setup = ((BattleSetup * (__fastcall*)(BattleSetup*))_addr(0x14024af90))(param);
    battle_setup->damage_disp = (std::byte)0x1;
    return battle_setup;
}

void SimulateFrame(sMvc3Main* param) {
    sMvc3Manager* manager;

    ((void* (__fastcall*)(void*))_addr(0x1405bb710))(param->mpCollision);


    manager = ((sMvc3Manager * (__fastcall*)())_addr(0x140001af0))();
    ((void* (__fastcall*)(void*))_addr(0x14014f410))(manager->mpAction);

    // Keyboards and mousesies
    manager = ((sMvc3Manager * (__fastcall*)())_addr(0x140001af0))();
    ((void* (__fastcall*)(void*))manager->mpShot->vtable->update2)(manager->mpShot);


    ((void* (__fastcall*)(void*))param->mpUnit->vtable->update)(param->mpUnit);
    ((void* (__fastcall*)(void*))param->mpCollision->vtable->update)(param->mpCollision);

    manager = ((sMvc3Manager * (__fastcall*)())_addr(0x140001af0))();
    ((void* (__fastcall*)(void*))manager->mpHitSolver->vtable->update)(manager->mpHitSolver);


    ((void* (__fastcall*)(void*))param->mpUnit->vtable->update3)(param->mpUnit);
    ((void* (__fastcall*)(void*))param->mpUnit->vtable->update2)(param->mpUnit);


    manager = ((sMvc3Manager * (__fastcall*)())_addr(0x140001af0))();
    ((void* (__fastcall*)(void*))manager->mpShot->vtable->update)(manager->mpShot);


    ((void* (__fastcall*)(void*))_addr(0x140011320))(manager->mpHitSolver);

    ((void* (__fastcall*)(void*))param->mpCollision->vtable->update2)(param->mpCollision);
}


void HookRenderFunction(sMvc3Main* param)
{
    sMvc3Manager* manager;


    SimulateFrame(param);
    SimulateFrame(param);
    SimulateFrame(param);
    SimulateFrame(param);
    ((void* (__fastcall*)(sMvc3Main*))_addr(0x1402594a0))(param);
    
    return;
}

void OnInitializeHook()
{

    Trampoline* tramp = Trampoline::MakeTrampoline(GetModuleHandle(nullptr));
    //InjectHook(_addr(0x14024d72f), tramp->Jump(HookBattleSetup), PATCH_JUMP);
    Patch(_addr(0x14051dfac), 0x48);
    Patch(_addr(0x14051dfad), 0x8b);
    Patch(_addr(0x14051dfae), 0xcf);
    InjectHook(_addr(0x14051dfaf), tramp->Jump(HookRenderFunction), PATCH_CALL); // DOn't forget to copy across the assignment to RCX
    Nop(_addr(0x14051dfb4), 1);
    
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
            OnInitializeHook();
        }
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

