// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "Trampoline.h"
#include "umvc3utils.h"
#include "MemoryMgr.h"
#include "umvc3_sMvc3Main.h"
#include "mvc3_frame_simulation.h"
#include <iostream>
#include <string>
#include "libs/ggponet.h"

using namespace Memory::VP;

bool begin_game(const char*) {
    printf("GGPO SAYS GO");
    Mvc3FrameSimulation::startGame();
    return true;
}

bool advance_frame(int) {
    return true;
}

bool load_game_state(unsigned char* buffer, int len)
{
    return true;
}


bool save_game_state(unsigned char** buffer, int* len, int* checksum, int)
{
    return true;
}

void free_buffer(void* buffer) {

}

bool log_game_state(char* filename, unsigned char* buffer, int)
{
    return true;
}

bool on_event_callback(GGPOEvent* info)
{
    return true;
}

void onGameReady() {
    printf("GAME READY!!!");
    GGPOSession * ggpo = NULL;
    GGPOErrorCode result;
    GGPOSessionCallbacks cb = {};

    cb.begin_game = begin_game;
    cb.advance_frame = advance_frame;
    cb.load_game_state = load_game_state;
    cb.save_game_state = save_game_state;
    cb.free_buffer = free_buffer;
    cb.log_game_state = log_game_state;
    cb.on_event = on_event_callback;


    char name[5] = { 'u','m','v','c','3' };

    /*
        result = ggpo_start_session(&ggpo,         // the new session object
        &cb,           // our callbacks
        "test_app",    // application name
        2,             // 2 players
        sizeof(int),   // size of an input packet
        8001);         // our local udp port
    */
    result = ggpo_start_synctest(&ggpo, &cb, name, 2, sizeof(int), 1);
}

DWORD WINAPI Initialise(LPVOID lpreserved) {

    AllocConsole();

    freopen("CONIN$", "r", stdin);
    freopen("CONOUT$", "w", stdout);
    freopen("CONOUT$", "w", stderr);

    printf("Initialise() | Begin!\n");
    for (std::string line; std::getline(std::cin, line);) {
        if (line == "ggpo") {
            std::cout << "GGPO TIME" << std::endl;
            Mvc3FrameSimulation::setToggleMode(1);
            Mvc3FrameSimulation::setFakePad(3, 1);

            Mvc3FrameSimulation::setPadToTeam(0, 0);

            Mvc3FrameSimulation::setPadToTeam(3, 1);

            Mvc3FrameSimulation::StartMatch();
            Mvc3FrameSimulation::OnGameReady(onGameReady);
        }
        if (line == "go") {
            std::cout << "GO TIME" << std::endl;
            Mvc3FrameSimulation::setToggleMode(1);
            Mvc3FrameSimulation::setFakePad(3, 1);

            Mvc3FrameSimulation::setPadToTeam(0, 0);

            Mvc3FrameSimulation::setPadToTeam(3, 1);

            Mvc3FrameSimulation::StartMatch();
            Mvc3FrameSimulation::startGame();
        }
        if (line == "alternate") {
            Mvc3FrameSimulation::setToggleMode(2);
            std::cout << "alternating render" << std::endl;
        }
        if (line == "normalrender") {
            Mvc3FrameSimulation::setToggleMode(0);
            std::cout << "normal render" << std::endl;
        }
        if (line == "myrender") {
            Mvc3FrameSimulation::setToggleMode(1);
            std::cout << "my render" << std::endl;
        }

        if (line == "record") {
            Mvc3FrameSimulation::startRecording(0);
            std::cout << "Recording!" << std::endl;
        }
        if (line == "recstop") {
            Mvc3FrameSimulation::stopRecording();
            std::cout << "Stopping" << std::endl;

        }
        if (line == "play") {
            Mvc3FrameSimulation::startPlaying(3);
            std::cout << "playing!" << std::endl;
        }
        if (line == "start") {
            Mvc3FrameSimulation::startGame();
        }
    }
    return TRUE;
}

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

BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        if (CheckGame()) {
            Mvc3FrameSimulation::InstallHook();
            CreateThread(nullptr, 0, Initialise, hModule, 0, nullptr);

        }
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

