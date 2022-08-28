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

struct sMvc3Manager {
    pointer vtable;
    char pad[0x30];
    pointer mpMvc3Setting;
    pointer mpBattleSetting;
    pointer mpAction;
    pointer mpCharacter;
    pointer mpStage;
    pointer mpGameEffect;
    sShot* mpShot;
    sHitSolver* mpHitSolver;
};


struct sMvc3Main {
    char sMain [0x40090];
    char field1_0x40090[256];
    void * mpRender; /* Created by retype action */
    void * mpTool; /* Created by retype action */
    sUnit * mpUnit; /* Created by retype action */
    void * mpResource; /* Created by retype action */
    void * mpCamera; /* Created by retype action */
    pointer mpNetwork; /* Created by retype action */
    pointer mpNetPad; /* Created by retype action */
    pointer mpAgent; /* Created by retype action */
    pointer mpGuide; /* Created by retype action */
    pointer mpEffect; /* Created by retype action */
   sCollision * mpCollision; /* Created by retype action */
    pointer mpShader2; /* Created by retype action */
    pointer mpSound; /* Created by retype action */
    pointer mpCapture; /* Created by retype action */
    pointer mpArea; /* Created by retype action */
    pointer mpGpuParticle; /* Created by retype action */
    pointer mpMouse; /* Created by retype action */
    pointer mpKeyboard; /* Created by retype action */
    pointer field20_0x40220;
    pointer mpId; /* Created by retype action */
    pointer mpVibration; /* Created by retype action */
    pointer field23_0x40238;
    pointer mpAI; /* Created by retype action */
    pointer mpScene; /* Created by retype action */
    pointer mpOccluder; /* Created by retype action */
    pointer mpShadow; /* Created by retype action */
    pointer mpPlatformUtil; /* Created by retype action */
    pointer field29_0x40268;
    pointer mpPrimitive; /* Created by retype action */
    pointer mpSyncPoint; /* Created by retype action */
    char field32_0x40280[53];
    char is_foreground; /* Created by retype action */
    char is_focused_window; /* Created by retype action */
    char field35_0x402b7[33];
    pointer mpMvc3Manager; /* Created by retype action */
};

