#include "pch.h"
#include "mvc3_frame_simulation.h"
#include "Trampoline.h"
#include "umvc3utils.h"
#include "MemoryMgr.h"
#include "umvc3_sMvc3Main.h"
#include <vector>
using namespace Memory::VP;


namespace Mvc3FrameSimulation {
    Mvc3GameState state;
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
    bool (*onLocalPlayerInput)(int, int) = nullptr;
    void (*onAdvanceFrameComplete)() = nullptr;
    void (*onFrameComplete)() = nullptr;
    int prevStep = 0;
    int nextInputP1 = -1;
    int nextInputP2 = -1;
    bool doStateUpdate = false;
    bool advanceFrameToNotify = false;


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

    // If cb returns false game frame will not advance
    void OnLocalPlayerInput(bool (*cb)(int, int)) {
        onLocalPlayerInput = cb;
    }

    void OnAdvanceFrameComplete(void (*cb)()) {
        onAdvanceFrameComplete = cb;
    }

    void OnFrameComplete(void (*cb)()) {
        onFrameComplete = cb;
    }

    void getInputs(sMvc3Main* mvc3Main) {
        ((void* (__fastcall*)(void*))_addr(0x140521df0))(&mvc3Main->sMain);
        ((void* (__fastcall*)(void*))_addr(0x140258540))(mvc3Main);
        ((void* (__fastcall*)(void*))_addr(0x140289c30))(mvc3Main->mpNetPad);
        OnReadInput(mvc3Main->mpNetPad);
    }

    void renderStuff(sMvc3Main* mvc3Main) {
        sMvc3Manager* manager;
        void* sRender;
        void* sPrimitive;
        void* sPad;

        struct Mystery {
            int unknown;
            int unknown2;
        };
        Mystery unknown;


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

        //Update30(mvc3Main->mpArea);


        // part 2
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


        manager = ((sMvc3Manager * (__fastcall*)())_addr(0x140001af0))();
        Update48(manager->mpGameEffect);

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

        manager = ((sMvc3Manager * (__fastcall*)())_addr(0x140001af0))();
        Update30(manager->mpGameEffect);




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

    void advanceFrame(sMvc3Main* mvc3Main) {
        printf("Starting Frame Advance\n");
        sMvc3Manager* manager;

        
        ((void* (__fastcall*)(void*))_addr(0x1405bb710))(mvc3Main->mpCollision);

        //ADded from render
        Update30(mvc3Main->mpArea);

        manager = ((sMvc3Manager * (__fastcall*)())_addr(0x140001af0))();
        ((void* (__fastcall*)(void*))_addr(0x14014f410))(manager->mpAction);
        

        manager = ((sMvc3Manager * (__fastcall*)())_addr(0x140001af0))();
        ((void* (__fastcall*)(void*))manager->mpShot->vtable->update2)(manager->mpShot);
        
        ((void* (__fastcall*)(void*))mvc3Main->mpUnit->vtable->update)(mvc3Main->mpUnit);
        ((void* (__fastcall*)(void*))mvc3Main->mpCollision->vtable->update)(mvc3Main->mpCollision);

        manager = ((sMvc3Manager * (__fastcall*)())_addr(0x140001af0))();
       ((void* (__fastcall*)(void*))manager->mpHitSolver->vtable->update)(manager->mpHitSolver);


        ((void* (__fastcall*)(void*))mvc3Main->mpUnit->vtable->update3)(mvc3Main->mpUnit); // UI?
        ((void* (__fastcall*)(void*))mvc3Main->mpUnit->vtable->update2)(mvc3Main->mpUnit);

        
        manager = ((sMvc3Manager * (__fastcall*)())_addr(0x140001af0))();
        ((void* (__fastcall*)(void*))manager->mpShot->vtable->update)(manager->mpShot);
   
        manager = ((sMvc3Manager * (__fastcall*)())_addr(0x140001af0))();
        ((void* (__fastcall*)(void*))_addr(0x140011320))(manager->mpHitSolver);
        
       ((void* (__fastcall*)(void*))mvc3Main->mpCollision->vtable->update2)(mvc3Main->mpCollision);
    
        
        if (onAdvanceFrameComplete != nullptr) {
            printf("Reporting frame advance complete\n");
            onAdvanceFrameComplete();
        }
        return;
    }

    void ReOrderedFrame(sMvc3Main* mvc3Main) {

        // Freeze the game if not ready to start
        sAction* action = *reinterpret_cast<sAction**>(_addr(0x140d47e68));
        //printf("Step: %x\n", action->mStep);
        if (action->mStep == 2) {
            if (prevStep != action->mStep) {
                if (onGameReady != nullptr) onGameReady();
                prevStep = action->mStep;
            }
            if (do_not_start == 1) return;
        }
        prevStep = action->mStep;


        // Read Inputs
        getInputs(mvc3Main);


        // Idk what this does
        Update30(mvc3Main->mpPlatformUtil);
        Update48(mvc3Main->mpResource);
        Update48(mvc3Main->mpTool);
        Update48(mvc3Main->mpEffect);

        // Send Local Input to GGPO
        int currentInput = mvc3Main->mpNetPad->mPad[0].data.On;
        int encoded = ((int(__fastcall*)(int, int, int))_addr(0x14002da30))(0, currentInput, 0);

        // If inputs are okay, or we haven't started the game yet, advance the frame

        if (action->mStep < 2 || !onLocalPlayerInput || onLocalPlayerInput && onLocalPlayerInput(0, encoded)) {
            // Okay to advance!!
            advanceFrame(mvc3Main);
        }

        // Render the game
        renderStuff(mvc3Main);

        if (onFrameComplete) {
            onFrameComplete();
        }
    }

    sMvc3Main* getSMain() {
        sMvc3Main* sMain = *reinterpret_cast<sMvc3Main**>(_addr(0x140e177e8));
        return sMain;
    }

    //drawMode 0 = no draw, 1 = draw and update, 2 = only draw
    void SimulateFrame(sMvc3Main* mvc3Main, int drawMode) {
        
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

        if (drawMode >= 1) {
            ((void* (__fastcall*)(void*))_addr(0x140521df0))(&mvc3Main->sMain);
            ((void* (__fastcall*)(void*))_addr(0x140258540))(mvc3Main);
            ((void* (__fastcall*)(void*))_addr(0x140289c30))(mvc3Main->mpNetPad);
            OnReadInput(mvc3Main->mpNetPad);
            
            Update30(mvc3Main->mpPlatformUtil);
            Update48(mvc3Main->mpResource);
            Update48(mvc3Main->mpTool);
            Update48(mvc3Main->mpEffect);
        }

        if (drawMode < 2) {
            ((void* (__fastcall*)(void*))_addr(0x1405bb710))(mvc3Main->mpCollision);
        }

        if (drawMode >= 1) {
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

        if (drawMode < 2) {
            manager = ((sMvc3Manager * (__fastcall*)())_addr(0x140001af0))();
            ((void* (__fastcall*)(void*))_addr(0x14014f410))(manager->mpAction);
        }

        if (drawMode >= 1) {
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


        if (drawMode < 2) {
            manager = ((sMvc3Manager * (__fastcall*)())_addr(0x140001af0))();
            ((void* (__fastcall*)(void*))manager->mpShot->vtable->update2)(manager->mpShot);
        }

        if (drawMode >= 1) {
            manager = ((sMvc3Manager * (__fastcall*)())_addr(0x140001af0))();
            Update48(manager->mpGameEffect);
        }

        if (drawMode < 2) {
            ((void* (__fastcall*)(void*))mvc3Main->mpUnit->vtable->update)(mvc3Main->mpUnit);
            ((void* (__fastcall*)(void*))mvc3Main->mpCollision->vtable->update)(mvc3Main->mpCollision);

            manager = ((sMvc3Manager * (__fastcall*)())_addr(0x140001af0))();
            ((void* (__fastcall*)(void*))manager->mpHitSolver->vtable->update)(manager->mpHitSolver);


            ((void* (__fastcall*)(void*))mvc3Main->mpUnit->vtable->update3)(mvc3Main->mpUnit);
            ((void* (__fastcall*)(void*))mvc3Main->mpUnit->vtable->update2)(mvc3Main->mpUnit);
        }

        if (drawMode >= 1) {
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

        if (drawMode < 2) {
            manager = ((sMvc3Manager * (__fastcall*)())_addr(0x140001af0))();
            ((void* (__fastcall*)(void*))manager->mpShot->vtable->update)(manager->mpShot);
        }

        if (drawMode >= 1) {
            manager = ((sMvc3Manager * (__fastcall*)())_addr(0x140001af0))();
            Update30(manager->mpGameEffect);
        }

        if (drawMode < 2) {
            manager = ((sMvc3Manager * (__fastcall*)())_addr(0x140001af0))();
            ((void* (__fastcall*)(void*))_addr(0x140011320))(manager->mpHitSolver);
        }

        if (drawMode >= 1) {
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

        if (drawMode < 2) {
            ((void* (__fastcall*)(void*))mvc3Main->mpCollision->vtable->update2)(mvc3Main->mpCollision);
        }

        if (drawMode >= 1) {

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
        if (onAdvanceFrameComplete != nullptr) {
           // onAdvanceFrameComplete();
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
        case 3:
            ReOrderedFrame(param);
            return;
        case 4:
            ReOrderedFrame(param);
            advanceFrame(param);
            return;
        }
    }

    void setToggleMode(int tm) {
        toggleMode = tm;
        return;
    }

    void setFakePad(int idx, int ls) {
        padInfo[idx].fakepad = ls;
        return;
    }

    void setNextInputP1(int input) {
        nextInputP1 = input;
    }

    void setNextInputP2(int input) {
        nextInputP2 = input;
    }

    void queueStateUpdate() {
        doStateUpdate = true;
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

        int characterId = 0xf;

        ((void* (__fastcall*)(void*, int))_addr(0x140146560))(&(sBS->characters[0]), characterId); // Set Characters
        sBS->characters[1].mBody = 0x02;
        sBS->characters[1].assist = 0x02;
        sBS->characters[1].mType = characterId;
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
            onLocalPlayerInput(teamId, character->mInput.On);
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
                nextInputP1 = recording.at(recordingIndex).p1;
                recordingIndex++;
            }
        }

        if (nextInputP1 != -1 && teamId == 0) {
            printf("Injecting input: %x", nextInputP1);
            character->mInput.On = nextInputP1;
            character->mInput.mDat = nextInputP1;
            nextInputP1 = -1;
        }
        if (nextInputP2 != -1 && teamId == 1) {
            character->mInput.On = nextInputP2;
            nextInputP2 = -1;
        }
    }

    void AdvanceFrame() {
        //sAction* action = *reinterpret_cast<sAction**>(_addr(0x140d47e68));
        //((void * (__fastcall*)(sAction*))_addr(0x140006d20))(action);
       
        advanceFrame(getSMain());
    }

    Mvc3GameState* getState() {
        printf("Getting state\n");
        Mvc3GameState* currentState = (Mvc3GameState*)malloc(sizeof(Mvc3GameState));

        sMvc3Manager* manager = ((sMvc3Manager * (__fastcall*)())_addr(0x140001af0))();
        sAction* action = *reinterpret_cast<sAction**>(_addr(0x140d47e68));

        
        memcpy(&currentState->sChar, manager->mpCharacter, sizeof(sCharacter));
        memcpy(&currentState->c0, ((sCharacter*)manager->mpCharacter)->mTeam[0].point_char, sizeof(uCharacter));
        memcpy(&currentState->c3, ((sCharacter*)manager->mpCharacter)->mTeam[1].point_char, sizeof(uCharacter));

        memcpy(&currentState->action, action, sizeof(sAction));
        return currentState;
    }
   
    void setState(Mvc3GameState* state) {
        printf("Setting state\n");

        sMvc3Manager* manager = ((sMvc3Manager * (__fastcall*)())_addr(0x140001af0))();
        sCharacter* sChar = (sCharacter*)manager->mpCharacter;
        sAction* action = *reinterpret_cast<sAction**>(_addr(0x140d47e68));



        memcpy(manager->mpCharacter, &state->sChar, sizeof(sCharacter));

        memcpy(sChar->mTeam[0].point_char, &state->c0, sizeof(uCharacter));

        memcpy(sChar->mTeam[1].point_char, &state->c3, sizeof(uCharacter));
        memcpy(action, &state->action, sizeof(sAction));

        return;
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