#include "pch.h"
#include "mvc3_frame_simulation.h"
#include "Trampoline.h"
#include "umvc3utils.h"
#include "MemoryMgr.h"
#include "umvc3_sMvc3Main.h"
using namespace Memory::VP;


namespace Mvc3FrameSimulation {
    int toggle = 0;
    int toggleMode = 0;
    int padLifeSupport = 0;
     
    void Update30(Updatable* updatable) {
        ((void* (__fastcall*)(void*))updatable->vtable->update)(updatable);
    }

    void Update48(Updatable* updatable) {
        ((void* (__fastcall*)(void*))updatable->vtable->update2)(updatable);
    }

    void Update50(Updatable* updatable) {
        ((void* (__fastcall*)(void*))updatable->vtable->update50)(updatable);
    }

    struct Mystery{
        int unknown;
        int unknown2;
    };

    void SimulateFrame(sMvc3Main* mvc3Main, int full) {
        //1402594a0
        sMvc3Manager* manager;
        void* sRender;
        void* sPrimitive;
        void* sPad;
        Mystery unknown;

        if (full == 1) {
            ((void* (__fastcall*)(void*))_addr(0x140521df0))(&mvc3Main->sMain);
            ((void* (__fastcall*)(void*))_addr(0x140258540))(mvc3Main);
            ((void* (__fastcall*)(void*))_addr(0x140289c30))(mvc3Main->mpNetPad);
            if (padLifeSupport == 1) {
                for (int i = 0; i < 4; i++) {
                    //mvc3Main->mpNetPad->mPad[i].rno = 1;
                    mvc3Main->mpNetPad->mPad[i].kind = 4;
                    //mvc3Main->mpNetPad->mPad[i].ability = 0x23;
                    /*
                    if (mvc3Main->mpNetPad->mPad[i].input_attr == 0x8) {
                        mvc3Main->mpNetPad->mPad[i].input_attr = 0x18;
                    }
                    if (mvc3Main->mpNetPad->mPad[i].socket_no == 0x00) {
                        mvc3Main->mpNetPad->mPad[i].socket_no == 0x77;
                    }
                    */
                }
            }
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
            ((void* (__fastcall*)(void*, void*))_addr(0x14020c7d0))(sRender, &unknown); //Is this right?

            sPrimitive = ((void* (__fastcall*)())_addr(0x1401e0ca0))(); // get sPrimitive
            ((void* (__fastcall*)(void*, int))_addr(0x140932790))(sPrimitive, unknown.unknown); //Is this right either?
            sPrimitive = ((void* (__fastcall*)())_addr(0x1401e0ca0))(); // get sPrimitive
            ((void* (__fastcall*)(void*, int))_addr(0x140447110))(sPrimitive, unknown.unknown2); // wtf? 

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
            ((void* (__fastcall*)(void*))mvc3Main->mpNetPad->vtable->update)(mvc3Main->mpNetPad);
            Update30(mvc3Main->mpMouse);
            Update30(mvc3Main->mpId);
            Update30(mvc3Main->mpKeyboard);
            ((void* (__fastcall*)(void*))mvc3Main->mpNetPad->vtable->update50)(mvc3Main->mpNetPad);
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


        manager = ((sMvc3Manager * (__fastcall*)())_addr(0x140001af0))();
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

            ((void* (__fastcall*)(void*))_addr(0x140259ea0))(mvc3Main);

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
                    void* sMain = ((void* (__fastcall*)())_addr(0x0140003150))();
                    ((void* (__fastcall*)(void*))_addr(0x1405213a0))(sMain);
                }

            }
        }
        return;
    }

    void HookRenderFunction(sMvc3Main* param)
    {
        switch (toggleMode) {
        case 0:
            ((void* (__fastcall*)(sMvc3Main*))_addr(0x1402594a0))(param);
            return;
        case 1:
            SimulateFrame(param, 1);
            return;
        case 2:
            if (toggle == 1) {
                SimulateFrame(param, 1);
                toggle = 0;
            }
            else {
                ((void* (__fastcall*)(sMvc3Main*))_addr(0x1402594a0))(param);
                toggle = 1;
            }
            return;
        }
        return;
    }

    void setToggleMode(int tm) {
        toggleMode = tm;
        return;
    }

    void setLifeSupport(int ls) {
        padLifeSupport = ls;
        return;
    }

    void InstallHook()
    {

        Trampoline* tramp = Trampoline::MakeTrampoline(GetModuleHandle(nullptr));
        Patch(_addr(0x14051dfac), 0x48);
        Patch(_addr(0x14051dfad), 0x8b);
        Patch(_addr(0x14051dfae), 0xcf);
        InjectHook(_addr(0x14051dfaf), tramp->Jump(HookRenderFunction), PATCH_CALL);
        Nop(_addr(0x14051dfb4), 1);

    }
}