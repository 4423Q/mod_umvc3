// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "Trampoline.h"
#include "umvc3utils.h"
#include "MemoryMgr.h"
#include "umvc3_sMvc3Main.h"
#include "mvc3_frame_simulation.h"
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

void Update30(Updatable * updatable) {
    ((void* (__fastcall*)(void*))updatable->vtable->update)(updatable);
}

void Update48(Updatable* updatable) {
    ((void* (__fastcall*)(void*))updatable->vtable->update2)(updatable);
}

void Update50(Updatable* updatable) {
    ((void* (__fastcall*)(void*))updatable->vtable->update50)(updatable);
}


void SimulateFrame(sMvc3Main* mvc3Main, int full) {
    //1402594a0
    sMvc3Manager* manager;
    void* sRender;
    void* sPrimitive;
    void* sPad;
    int unknown;
    int unknown2 = 0;

    if (full == 1) {
        ((void* (__fastcall*)(void*))_addr(0x140521df0))(mvc3Main);
        ((void* (__fastcall*)(void*))_addr(0x140258540))(mvc3Main);
        ((void* (__fastcall*)(void*))_addr(0x140289c30))(mvc3Main->mpNetPad);
        Update30(mvc3Main->mpPlatformUtil);
        Update48(mvc3Main->mpResource);
        Update48(mvc3Main->mpTool);
        Update48(mvc3Main->mpEffect);
    }

    ((void* (__fastcall*)(void*))_addr(0x1405bb710))(mvc3Main->mpCollision);

    if (full == 1) {
        manager = ((sMvc3Manager * (__fastcall*)())_addr(0x140001af0))();
        Update48(manager->mpGameConfig);

        sRender = ((void* (__fastcall*)())_addr(0x1400047a0))(); // get srender
        ((void* (__fastcall*)(void*, int))_addr(0x14053de60))(sRender, 2);
        sRender = ((void* (__fastcall*)())_addr(0x1400047a0))(); // get srender
        ((void* (__fastcall*)(void*, int*))_addr(0x14020c7d0))(sRender, &unknown); //Is this right?

        sPrimitive = ((void* (__fastcall*)())_addr(0x1401e0ca0))(); // get sPrimitive
        ((void* (__fastcall*)(void*, int))_addr(0x140932790))(sPrimitive, unknown); //Is this right either?
        sPrimitive = ((void* (__fastcall*)())_addr(0x1401e0ca0))(); // get sPrimitive
        ((void* (__fastcall*)(void*, int))_addr(0x140447110))(sPrimitive, unknown2); // wtf? 

        Update48(mvc3Main->mpPrimitive);
        Update48(mvc3Main->mpGpuParticle);
        ((void* (__fastcall*)(void*))_addr(0x140537080))(mvc3Main->mpRender);

        Update30(mvc3Main->mpArea);
    }

    manager = ((sMvc3Manager * (__fastcall*)())_addr(0x140001af0))();
    ((void* (__fastcall*)(void*))_addr(0x14014f410))(manager->mpAction);

    if (full == 1) {
        // Keyboards and mousesies
        Update30(mvc3Main->mpVibration);
        Update30(mvc3Main->mpNetPad);
        Update30(mvc3Main->mpId);
        Update30(mvc3Main->mpKeyboard);
        Update50(mvc3Main->mpNetPad);
        Update30(mvc3Main->mpShader2);
        Update30(mvc3Main->mpAI);

        manager = ((sMvc3Manager * (__fastcall*)())_addr(0x140001af0))();
        Update30(manager->mpGameConfig);
        manager = ((sMvc3Manager * (__fastcall*)())_addr(0x140001af0))();
        Update30(manager->mpGameUi);
    }


    manager = ((sMvc3Manager * (__fastcall*)())_addr(0x140001af0))();
    ((void* (__fastcall*)(void*))manager->mpShot->vtable->update2)(manager->mpShot);

    if (full == 1) {
        manager = ((sMvc3Manager * (__fastcall*)())_addr(0x140001af0))();
        Update48(manager->mpGameEffect);
    }


    ((void* (__fastcall*)(void*))mvc3Main->mpUnit->vtable->update)(mvc3Main->mpUnit);
    ((void* (__fastcall*)(void*))mvc3Main->mpCollision->vtable->update)(mvc3Main->mpCollision);

    manager = ((sMvc3Manager * (__fastcall*)())_addr(0x140001af0))();
    ((void* (__fastcall*)(void*))manager->mpHitSolver->vtable->update)(manager->mpHitSolver);


    ((void* (__fastcall*)(void*))mvc3Main->mpUnit->vtable->update3)(mvc3Main->mpUnit);
    ((void* (__fastcall*)(void*))mvc3Main->mpUnit->vtable->update2)(mvc3Main->mpUnit);

    if (full == 1) {
        ((void* (__fastcall*)(void*))_addr(0x140647910))(mvc3Main->mpAI);
        Update30(mvc3Main->mpEffect);
        Update30(mvc3Main->mpGpuParticle);
        Update30(mvc3Main->mpCamera);
        Update30(mvc3Main->mpSound);
        Update30(mvc3Main->mpScene);
        Update30(mvc3Main->mpShadow);

        manager = ((sMvc3Manager * (__fastcall*)())_addr(0x140001af0))();
        ((void* (__fastcall*)(void*))manager->vtable->update)(manager);

        manager = ((sMvc3Manager * (__fastcall*)())_addr(0x140001af0))();
        Update30(manager->mpCharacter);

        manager = ((sMvc3Manager * (__fastcall*)())_addr(0x140001af0))();
        Update30(manager->mpStage);
    }


    manager = ((sMvc3Manager * (__fastcall*)())_addr(0x140001af0))();
    ((void* (__fastcall*)(void*))manager->mpShot->vtable->update)(manager->mpShot);

    if (full == 1) {
        manager = ((sMvc3Manager * (__fastcall*)())_addr(0x140001af0))();
        Update30(manager->mpGameEffect);
    }


    ((void* (__fastcall*)(void*))_addr(0x140011320))(manager->mpHitSolver);

    if (full == 1) {
        manager = ((sMvc3Manager * (__fastcall*)())_addr(0x140001af0))();
        Update30(manager->mpReplay);
        manager = ((sMvc3Manager * (__fastcall*)())_addr(0x140001af0))();
        Update30(manager->mpGSound);
        manager = ((sMvc3Manager * (__fastcall*)())_addr(0x140001af0))();
        Update30(manager->mpBattleSetting);
        manager = ((sMvc3Manager * (__fastcall*)())_addr(0x140001af0))();
        Update30(manager->mpMvc3Setting);
        manager = ((sMvc3Manager * (__fastcall*)())_addr(0x140001af0))();
        Update30(manager->mpNetwork);
        manager = ((sMvc3Manager * (__fastcall*)())_addr(0x140001af0))();
        Update30(manager->mpLicense);

        Update30(mvc3Main->mpNetwork);
        Update30(mvc3Main->mpAgent);
        Update30(mvc3Main->mpGuide);

        manager = ((sMvc3Manager * (__fastcall*)())_addr(0x140001af0))();
        Update30(manager->mpRanking);

        manager = ((sMvc3Manager * (__fastcall*)())_addr(0x140001af0))();
        Update30(manager->mpNetworkRoom);

        manager = ((sMvc3Manager * (__fastcall*)())_addr(0x140001af0))();
        Update30(manager->mpEarthAttack);

        Update30(mvc3Main->mpTool);
        Update30(mvc3Main->mpPrimitive);
    }

    ((void* (__fastcall*)(void*))mvc3Main->mpCollision->vtable->update2)(mvc3Main->mpCollision);

    if (full == 1) {

        ((void* (__fastcall*)(void*))_addr(0x140259ea0))(mvc3Main); // render menus?

        Update30(mvc3Main->mpRender);
        ((void* (__fastcall*)(void*))_addr(0x14053a250))(mvc3Main->mpRender);
        Update50(mvc3Main->mpGpuParticle);
        Update50(mvc3Main->mpPrimitive);
        Update30(mvc3Main->mpCapture);


        sPad = ((void* (__fastcall*)())_addr(0x140001b00))();
        int result1 = ((int(__fastcall*)(void*, int, char))_addr(0x1402b3d00))(sPad, 0xa4, 1);

        sPad = ((void* (__fastcall*)())_addr(0x140001b00))();
        int result2 = ((int(__fastcall*)(void*, int, char))_addr(0x1402b3d00))(sPad, 0xa5, 1);
        if ((result1 != 0) || (result2 != 0)) {

            sPad = ((void* (__fastcall*)())_addr(0x140001b00))();
            int result3 = ((int(__fastcall*)(void*, int, char))_addr(0x1402b3dc0))(sPad, 0x73, 1);

            if (result3 != 0) {       
                void * sMain = ((void* (__fastcall*)())_addr(0x0140003150))();
                ((void* (__fastcall*)(void*))_addr(0x1405213a0))(sMain);
            }

        }
    }
    return;
}


void HookRenderFunction(sMvc3Main* param)
{

    SimulateFrame(param, 1);
    //((void* (__fastcall*)(sMvc3Main*))_addr(0x1402594a0))(param);
    
    return;
}

void OnInitializeHook()
{

    Trampoline* tramp = Trampoline::MakeTrampoline(GetModuleHandle(nullptr));
    Patch(_addr(0x14051dfac), 0x48);
    Patch(_addr(0x14051dfad), 0x8b);
    Patch(_addr(0x14051dfae), 0xcf);
    InjectHook(_addr(0x14051dfaf), tramp->Jump(HookRenderFunction), PATCH_CALL);
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
            //OnInitializeHook();
            Mvc3FrameSimulation::InstallHook();
        }
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}
