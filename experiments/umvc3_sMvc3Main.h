#pragma once    
typedef struct sMvc3Main sMvc3Main, *PsMvc3Main;

typedef void* pointer;

struct sUnit_vtable {
    char pad[0x30];
    void* update;
    char pad_[0x20];
    void* update2;
    char pad__[0x8];
    void* update3;
};

struct sUnit {
    sUnit_vtable* vtable;
};

struct sCollision_vtable {
    char pad[0x30];
    void* update;
    char pad_[0x10];
    void* update2;
};

struct sCollision {
    sCollision_vtable* vtable;
};

struct sHitSolver_vtable {
    char pad[0x30];
    void* update;
};

struct sHitSolver {
    sHitSolver_vtable* vtable;
};

struct sShot_vtable {
    char pad[0x30];
    void* update;
    char pad_[0x10];
    void* update2;
};

struct sShot {
    sShot_vtable* vtable;
};

struct sMvc3Manager_vtable {
    char pad[0x30];
    void* update;
};

struct Updatable_vtable {
    char pad[0x30];
    void* update;
    char pad_[0x10];
    void* update2;
    void* update50;
};

struct Updatable {
    Updatable_vtable* vtable;
};


struct sMvc3NetPad {

    Updatable_vtable* vtable;
    char pad[0x50];
    struct sPad_pad {
        char vtable[8];
        char be_flag;
        char rno;
        char pad_no;
        char kind;
        char ability;
        char pad[0x224];
    };
    sPad_pad mPad[6];
};
 
struct sMvc3Manager {
    sMvc3Manager_vtable * vtable;
    char pad[0x30];
    Updatable* mpMvc3Setting;
    Updatable* mpBattleSetting;
    Updatable* mpAction;
    Updatable* mpCharacter;
    Updatable* mpStage;
    Updatable* mpGameEffect;
    sShot* mpShot;
    sHitSolver* mpHitSolver;
    Updatable* mpGameConfig;
    Updatable* mpGameUi;
    Updatable* mpGSound;
    Updatable* mpReplay;
    Updatable* mpRanking;
    Updatable* mpMission;
    Updatable* mpLicense;
    Updatable* mpNetwork;
    Updatable* mpNetworkRoom;
    Updatable* mpMovie;
    Updatable* mpEarthAttack;
};




struct sMvc3Main {
    char sMain [0x40090];
    char field1_0x40090[256];
    Updatable* mpRender; /* Created by retype action */
    Updatable * mpTool; /* Created by retype action */
    sUnit * mpUnit; /* Created by retype action */
    Updatable * mpResource; /* Created by retype action */
    Updatable * mpCamera; /* Created by retype action */
    Updatable* mpNetwork; /* Created by retype action */
    sMvc3NetPad* mpNetPad; /* Created by retype action */
    Updatable* mpAgent; /* Created by retype action */
    Updatable* mpGuide; /* Created by retype action */
    Updatable * mpEffect; /* Created by retype action */
   sCollision * mpCollision; /* Created by retype action */
    Updatable* mpShader2; /* Created by retype action */
    Updatable * mpSound; /* Created by retype action */
    Updatable* mpCapture; /* Created by retype action */
    Updatable* mpArea; /* Created by retype action */
    Updatable* mpGpuParticle; /* Created by retype action */
    Updatable* mpMouse; /* Created by retype action */
    Updatable* mpKeyboard; /* Created by retype action */
    pointer field20_0x40220;
    Updatable* mpId; /* Created by retype action */
    Updatable* mpVibration; /* Created by retype action */
    pointer field23_0x40238;
    Updatable* mpAI; /* Created by retype action */
    Updatable *  mpScene; /* Created by retype action */
    pointer mpOccluder; /* Created by retype action */
    Updatable * mpShadow; /* Created by retype action */
    Updatable * mpPlatformUtil; /* Created by retype action */
    pointer field29_0x40268;
    Updatable * mpPrimitive; /* Created by retype action */
    pointer mpSyncPoint; /* Created by retype action */
    char field32_0x40280[53];
    char is_foreground; /* Created by retype action */
    char is_focused_window; /* Created by retype action */
    char field35_0x402b7[33];
    pointer mpMvc3Manager; /* Created by retype action */
};
