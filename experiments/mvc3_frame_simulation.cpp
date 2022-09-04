#include "pch.h"
#include "mvc3_frame_simulation.h"
#include "Trampoline.h"
#include "umvc3utils.h"
#include "MemoryMgr.h"
#include "umvc3_sMvc3Main.h"
#include <vector>
using namespace Memory::VP;


namespace Mvc3FrameSimulation {
    struct RecordingItem {
        int p1;
        int p2;
    };
    int toggle = 0;
    int toggleMode = 0;
    int recordingMode = 0;
    std::vector<RecordingItem> recording;
    int recordingIndex;
    int maxRecordingIndex = 60 * 180;
    int do_not_start = 0;
    void (* onGameReady)() = nullptr;
    void (*onLocalPlayerInput)(int, int) = nullptr;
    int prevStep = 0;

    struct PadInfo {
        int fakepad;
        int teamId;
    };
    PadInfo padInfo[4] = {
        { 0, -1, },
        { 0, -1, },
        { 0, -1, },
        { 0, -1, }
    };
     
    void Update30(Updatable* updatable) {
        ((void* (__fastcall*)(void*))updatable->vtable->update)(updatable);
    }

    void Update48(Updatable* updatable) {
        ((void* (__fastcall*)(void*))updatable->vtable->update2)(updatable);
    }

    void Update50(Updatable* updatable) {
        ((void* (__fastcall*)(void*))updatable->vtable->update50)(updatable);
    }

    void startRecording(int pad_idx) {
        recordingIndex = 0;
        recording.clear();
        recording.push_back({ -1, -1 });
        recordingMode = 1;
    }

    void stopRecording() {
        recordingMode = 0;
    }

    void startPlaying(int pad_idx) {
        stopRecording();
        recordingIndex = 0;
        recordingMode = 2;
    }

    void stopPlaying() {
        recordingMode = 0;
    }

    void setPadToTeam(int pad_idx, int team_idx) {
        padInfo[pad_idx].teamId = team_idx;
        sMvc3Manager* manager = ((sMvc3Manager * (__fastcall*)())_addr(0x140001af0))();
        manager->mpPadId[team_idx] = pad_idx;
    }

    void OnReadInput(sMvc3NetPad* netPad) {
        for (int i = 0; i < 4; i++) {
            if (padInfo[i].fakepad) {
                netPad->mPad[i].kind = 4;
            }
        }
    }

    void startGame() {
        do_not_start = 0;
    }

    void OnGameReady(void (*cb)()) {
        onGameReady = cb;
    }

    void SimulateFrame(sMvc3Main* mvc3Main, int full) {
        //1402594a0
        sMvc3Manager* manager;
        void* sRender;
        void* sPrimitive;
        void* sPad;
        struct Mystery {
            int unknown;
            int unknown2;
        };
        Mystery unknown;

        sAction* action = *reinterpret_cast<sAction**>(_addr(0x140d47e68));
        printf("Step: %x\n", action->mStep);
        if (action->mStep == 2) {
            if (prevStep != action->mStep) {
                if (onGameReady != nullptr) onGameReady();
                prevStep = action->mStep;
            }
            if (do_not_start == 1) return;
        }
        prevStep = action->mStep;

        if (full == 1) {
            ((void* (__fastcall*)(void*))_addr(0x140521df0))(&mvc3Main->sMain);
            ((void* (__fastcall*)(void*))_addr(0x140258540))(mvc3Main);
            ((void* (__fastcall*)(void*))_addr(0x140289c30))(mvc3Main->mpNetPad);
            OnReadInput(mvc3Main->mpNetPad);
            
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
            ((void* (__fastcall*)(void*, void*))_addr(0x14020c7d0))(sRender, &unknown);

            sPrimitive = ((void* (__fastcall*)())_addr(0x1401e0ca0))(); // get sPrimitive
            ((void* (__fastcall*)(void*, int))_addr(0x140932790))(sPrimitive, unknown.unknown); 
            sPrimitive = ((void* (__fastcall*)())_addr(0x1401e0ca0))(); // get sPrimitive
            ((void* (__fastcall*)(void*, int))_addr(0x140447110))(sPrimitive, unknown.unknown2); 

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

    void setFakePad(int idx, int ls) {
        padInfo[idx].fakepad = ls;
        return;
    }

    void StartMatch() {
        do_not_start = 1;
        sBattleSetting* sBS;
        sBS = ((sBattleSetting* (__fastcall*)())_addr(0x140004700))();
        
        ((void* (__fastcall*)(void*, int))_addr(0x14024e050))(sBS, 0); // Set Game Mode to versus
        ((void* (__fastcall*)(void*, int))_addr(0x140146530))(sBS, 0xb4); // Set time limit
        
        // Uses the actual stage IDs, for indexes in the list use: 1402bc5d0
        ((void* (__fastcall*)(void*, int))_addr(0x1401cf670))(sBS, 0x69); // Set Stage

        //Should probably also wipe out the handicaps and damage settings but cba to figure out how

        int characterId = 0x23;

        ((void* (__fastcall*)(void*, int))_addr(0x140146560))(&(sBS->characters[0]), characterId); // Set Characters
        sBS->characters[1].mBody = 0x02;
        sBS->characters[1].assist = 0x02;
        sBS->characters[1].mType = characterId + 1;
       // ((void* (__fastcall*)(void*, int))_addr(0x140146560))(&(sBS->characters[1]), characterId);
        ((void* (__fastcall*)(void*, int))_addr(0x140146560))(&(sBS->characters[2]), characterId + 2);
        ((void* (__fastcall*)(void*, int))_addr(0x140146560))(&(sBS->characters[3]), characterId + 3);
        ((void* (__fastcall*)(void*, int))_addr(0x140146560))(&(sBS->characters[4]), characterId + 4);
        ((void* (__fastcall*)(void*, int))_addr(0x140146560))(&(sBS->characters[5]), characterId + 5);

        ((void* (__fastcall*)(void*))_addr(0x14024b530))(sBS); // Jump into match
    }

    void OnPostInput(uCharacter* character) {
        int teamId = character->mTeamId;
        if (onLocalPlayerInput) {
            onLocalPlayerInput(teamId, 0);
        }
        if (recordingMode == 1) {
            if (recordingIndex >= maxRecordingIndex) {
                stopRecording();
            }
            RecordingItem back = recording.back();
            if (teamId == 0 && back.p1 == -1) {
                back.p1 = character->mInput.On;
            }
            if (teamId == 1 && back.p2 == -1) {
                back.p2 = character->mInput.On;
            }
            recording.at(recording.size() - 1) = back;
            if (back.p1 != -1 && back.p2 != -1) {
                recordingIndex++;
                recording.push_back({ -1, -1 });
            }
        }
        else if (recordingMode == 2) {
            if (recordingIndex >= recording.size()) {
                stopPlaying();
                return;
            }
            if (teamId == 0) {
                character->mInput.On = recording.at(recordingIndex).p1;
                recordingIndex++;
            }
        }
    }

    int HookPostInputUpdate(uCharacter* character) {
        OnPostInput(character);

        return character->mTeamId;
    }

    void InstallInputHook()
    {
        Trampoline* tramp = Trampoline::MakeTrampoline(GetModuleHandle(nullptr));
        InjectHook(_addr(0x14002e280), tramp->Jump(HookPostInputUpdate), PATCH_CALL);
    }

    void InstallHook()
    {
        Trampoline* tramp = Trampoline::MakeTrampoline(GetModuleHandle(nullptr));
        Patch(_addr(0x14051dfac), 0x48);
        Patch(_addr(0x14051dfad), 0x8b);
        Patch(_addr(0x14051dfae), 0xcf);
        InjectHook(_addr(0x14051dfaf), tramp->Jump(HookRenderFunction), PATCH_CALL);
        Nop(_addr(0x14051dfb4), 1);
        InstallInputHook();
    }
}